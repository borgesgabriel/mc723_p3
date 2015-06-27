#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242981
#define ADDRESS_CORE_ZERO_WRITE 115243080

#define bool char
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int num_cores;
int processor_number = 0;
int m = 0;

void set_core(bool on, int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_WRITE + 2 * core_num + on);
  *p = on;
}

uint32_t number_of_cores() {
  uint32_t volatile* p = (uint32_t volatile*) NUMBER_OF_CORES_REQUEST;
  return *p >> 24; // número de "zeros" a serem cortados
}

bool is_core_on(int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_READ + core_num);
  return (bool)(*p >> 24);
}

int load_linked(int* ptr) {
  int rt;
  asm(
    "ll %0, (%1);"
    : "=r" (rt)
    : "r" (ptr)
  );
  return rt;
}

int store_conditional(int* ptr, int value) {
  asm(
    "sc %0, (%1);"
    : "=r" (value)
    : "r" (ptr)        
  );
  return value;
}

void mutex_lock(int* lock) {
  while (load_linked(lock) || !store_conditional(lock, 1)) {
    int i;
    for(i = 0; i < 10; ++i);
  }
}

void mutex_unlock(int* lock) {
  *lock = 0; 
}

int main(int argc, char *argv[]){
  int i, j;
  int pid = processor_number++;
  // for(i=0;i<8;++i)
  //   printf("%d ", is_core_on(i));
  // printf("\n");
  if (!pid) {
    for(i=1;i<2;++i) {
      set_core(1,i);
    }
  }
  for(i=0;i<10;i++) {
    mutex_lock(&m);
    printf("Hi from processor MIPS! %d\n", pid);
    mutex_unlock(&m);
  }
  //   for (j = 0; j < 10000; ++j);
  // }
  // for(i=0;i<8;++i)
  //   printf("%" PRIu32 " ", is_core_on(i));
  // printf("\n");

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

