#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#define SLEEP_TIME 100


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

int
main(int argc, char *argv[])
{
  
  lock_init(&lk);
  int arg1 = 1, arg2 = 1;

  fprintf(1, "below should be sequential print statements:\n");
  thread_create(&f1, (void *)&arg1, (void *)&arg2);
  thread_create(&f2, (void *)&arg1, (void *)&arg2);
  thread_create(&f3, (void *)&arg1, (void *)&arg2);
  thread_join();
  thread_join();
  thread_join();

  arg1 = 0;
  fprintf(1, "below should be a jarbled mess:\n");
  thread_create(&f1, (void *)&arg1, (void *)&arg2);
  thread_create(&f2, (void *)&arg1, (void *)&arg2);
  thread_create(&f3, (void *)&arg1, (void *)&arg2);
  thread_join();
  thread_join();
  thread_join();
  
  exit(0);
}
