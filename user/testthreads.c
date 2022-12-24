#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#define SLEEP_TIME 20


lock_t lk;

void f1(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "1. this should print %s\n", num ? "first" : "whenever");
  fprintf(1, "1. sleep for %d ticks\n", SLEEP_TIME);
  sleep(SLEEP_TIME);
  if (num) lock_release(&lk);
  exit(0);
}

void f2(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "2. this should print %s\n", num ? "second" : "whenever");
  fprintf(1, "2. sleep for %d ticks\n", SLEEP_TIME);
  sleep(SLEEP_TIME);
  if (num) lock_release(&lk);
  exit(0);
}

void f3(void* arg1, void* arg2) {
  int num = *(int*)arg1;
  if (num) lock_acquire(&lk);
  fprintf(1, "3. this should print %s\n", num ? "third" : "whenever");
  fprintf(1, "3. sleep for %d ticks\n", SLEEP_TIME);
  sleep(SLEEP_TIME);
  if (num) lock_release(&lk);
  exit(0);
}

void prueba(void* arg1, void* arg2){
  //int num = *(int*)(arg1);
  while(1){
    fprintf(1,"hey\n");
    printf("Arg1 en prueba: %p, valor: %d\n",arg1, *(int*)arg1);
    printf("Arg2 en prueba: %p, valor %d\n",arg2, *(int*)arg2);
    sleep(1000);
  }
  exit(0);
}

int a = 1, b = 2;

int
main(int argc, char *argv[])
{
  
  lock_init(&lk);

  //printf("Arg1 en user: %p\n",&a);
  //printf("Arg2 en user: %p\n",&b);

  //thread_create(&prueba,(void*)&a,(void*)&b);

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
  
  exit(0);
}
