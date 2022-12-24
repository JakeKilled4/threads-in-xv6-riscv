#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


lock_t lk;

void f1(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "Starting thread 1, this should print %s\n", num ? "first" : "whenever");

  for(int i= 0;i<10;i++){
    fprintf(1,"1 ");
    sleep(1);
  }

  fprintf(1,"\n");
  fprintf(1, "Thread 1 finish\n");
  sleep(1);
  if (num) lock_release(&lk);
  exit(0);
}

void f2(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "Starting thread 2, this should print %s\n", num ? "second" : "whenever");

  for(int i= 0;i<10;i++){
    fprintf(1,"2 ");
    sleep(1);
  }

  fprintf(1,"\n");
  fprintf(1, "Thread 2 finish\n");
  sleep(1);
  if (num) lock_release(&lk);
  exit(0);
}

void f3(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "Starting thread 3, this should print %s\n", num ? "third" : "whenever");

  for(int i= 0;i<10;i++){
    fprintf(1,"3 ");
    sleep(1);
  }

  fprintf(1,"\n");
  fprintf(1, "Thread 3 finish\n");
  sleep(1);
  if (num) lock_release(&lk);
  exit(0);
}


void bad_sum(void *arg1 , void * arg2 ){
  int n = *(int*)(arg2);
  for(int i = 0;i < n;i++){
    *(int*)arg1 += 1;
  }
  exit(0);
}


void good_sum(void *arg1 , void * arg2 ){
  int n = *(int*)(arg2);
  for(int i = 0;i < n;i++){
    lock_acquire(&lk);
    *(int*)arg1 += 1;
    lock_release(&lk);
  }
  exit(0);
}


int a = 1, b = 2;

int
main(int argc, char *argv[])
{
  
  lock_init(&lk);

  fprintf(1,"      First test\n");
  fprintf(1,"-------------------------\n");
  fprintf(1, "below should be sequential print statements:\n");
  thread_create(&f1, (void *)&a, (void *)&b);
  thread_create(&f2, (void *)&a, (void *)&b);
  thread_create(&f3, (void *)&a, (void *)&b);
  thread_join();
  thread_join();
  thread_join();

  a = 0;
  
  fprintf(1, "below should be a jarbled mess:\n");
  thread_create(&f1, (void *)&a, (void *)&b);
  thread_create(&f2, (void *)&a, (void *)&b);
  thread_create(&f3, (void *)&a, (void *)&b);
  thread_join();
  thread_join();
  thread_join();


  fprintf(1,"\n      Second test\n");
  fprintf(1,"-------------------------\n");
  
  a = 0;
  b = 1000000;

  fprintf(1,"Starting sum without locks\n");
  int num_threads = 50;
  for(int i = 0;i<num_threads;i++){
    thread_create(&bad_sum, (void *)&a, (void *)&b);
  }
  for(int i = 0;i<num_threads;i++){
    thread_join();
  }

  fprintf(1,"The resut is: %d, and should be: %d\n",a, num_threads*b);

  a = 0;
  b = 1000000;


  fprintf(1,"Starting sum with locks\n");

  for(int i = 0;i<num_threads;i++){
    thread_create(&good_sum, (void *)&a, (void *)&b);
  }
  for(int i = 0;i<num_threads;i++){
    thread_join();
  }

  fprintf(1,"The resut is: %d, and should be: %d\n",a, num_threads*b);
  
  exit(0);
}
