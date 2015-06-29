#include "../mypthread.h"

int num_cores;
pthread_mutex_t m = 0;

int main(int argc, char *argv[]){
  int i, j;
  pthread_mutex_lock(&m);
  int pid = processor_number++;
  pthread_mutex_unlock(&m);
  // for(i=0;i<8;++i)
  //   printf("%d ", is_core_on(i));
  // printf("\n");
  for(i=0;i<10;i++) {
    pthread_mutex_lock(&m);
    printf("Hi from processor MIPS! %d\n", pid);
    pthread_mutex_unlock(&m);
  }

  if (pid == 7) {
    for (i = 0; i < 7; ++i) {
      set_core(1, i);
    }
  } else {
    set_core(1, 1 + pid);
    set_core(0, pid);
  }
  pthread_mutex_lock(&m);
  printf("Exiting! %d\n", pid);
  pthread_mutex_unlock(&m);
  for(i = 0; i < 1000; ++i);
  //   for (j = 0; j < 10000; ++j);
  // }
  // for(i=0;i<8;++i)
  //   printf("%" PRIu32 " ", is_core_on(i));
  // printf("\n");

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

