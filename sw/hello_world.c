#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242981
#define ADDRESS_CORE_ZERO_WRITE 115243080
#define FIRST_EXECUTION 8000
#define ENTERING 8001
#define NUMBER 10001

#define bool char
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int num_cores;

uint32_t volatile getEntering(int i) {
  uint32_t volatile* p = (uint32_t volatile*) (ENTERING + i);
  return *p;
}

void setEntering(int i, uint32_t val) {
  uint32_t volatile* p = (uint32_t volatile*) (ENTERING + i);
  *p = val;
}

uint32_t volatile getNumber(int i) {
  uint32_t volatile* p = (uint32_t volatile*) (NUMBER + i);
  return *p;
}

void setNumber(int i, uint32_t val) {
  uint32_t volatile* p = (uint32_t volatile*) (NUMBER + i);
  *p = val;
}

int getPID() {
  return random() % 113;
}


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

void initialize() {
  uint32_t volatile* p = (uint32_t volatile*) FIRST_EXECUTION;
  if (*p != 1234) {
    *p = 1234;
    num_cores = number_of_cores();
    int i;
    for (i = 0; i < num_cores; ++i) {
      setEntering(i, 0);
      setNumber(i, 0);
    }
  }
}

void lock(int i) {
  setEntering(i, 1);
  uint32_t maxNumber = 0;
  int it;
  for (it = 0; it < num_cores; ++it) {
    maxNumber = MAX(maxNumber, getNumber(it));
  }
  setNumber(i, 1 + maxNumber);
  setEntering(i, 0);
  int j;
  for (j = 0; j < num_cores; ++j) {
    while (getEntering(j) == 1);
    while ((getNumber(j) != 0) && ( (getNumber(j) < getNumber(i)) || ((getNumber(j) == getNumber(i)) && (j < i))));
  }
}

void unlock(int i) {
  setNumber(i, 0);
}


int main(int argc, char *argv[]){
  initialize();
  int pid = getPID();
  int i, j;
  lock(pid);
  for(i=0;i<10;i++)
    printf("Hi from processor MIPS! Process PID is %d\n", pid);
  unlock(pid);
  lock(pid);
  for(i=0;i<8;++i)
    printf("%d ", is_core_on(i));
  printf("\n");
  unlock(pid);
  for(i=1;i<8;++i) {
    set_core(1,i);
    for (j = 0; j < 10000; ++j);
  }
  lock(pid);
  for(i=0;i<8;++i)
    printf("%" PRIu32 " ", is_core_on(i));
  printf("\n");
  unlock(pid);

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

