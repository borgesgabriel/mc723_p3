#ifndef MYPTHREAD_H
#define MYPTHREAD_H

/* includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "mypthread_helper.h"

/* helper functions */

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

void set_core(bool on, int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_WRITE + 4 * (2 * core_num + on));
  *p = on;
}

uint32_t number_of_cores() {
  uint32_t volatile* p = (uint32_t volatile*) NUMBER_OF_CORES_REQUEST;
  return *p >> 24; // número de "zeros" a serem cortados
}

bool is_core_on(int core_num) {
  uint32_t volatile* p = (uint32_t volatile*) (ADDRESS_CORE_ZERO_READ + 4 * core_num);
  return (bool)(*p >> 24);
}

long double read_piargs(int threadID) {
  int address = GET_RESULT_REQUEST + 4 * threadID;
  uint32_t volatile* p = (uint32_t volatile*) address;
  uint32_t volatile compute = *p;

  int i;
  for (i = 0; i < 100000; ++i);

  char filename[50];
  long double result;

  sprintf(filename, "core%d.in", threadID);
  if (OFFLOAD_DEBUG) {
    printf("piargs reading from %s\n", filename);
  }
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%Lf\n", &result);
  fclose(fp);

  return result;
}

void write_piargs(int threadID, double intervals, int numThreads) {
  char filename[50];

  sprintf(filename, "core%d.out", threadID);
  if (OFFLOAD_DEBUG) {
    printf("piargs writing on %s\n"
           "content: %d %d %32.30Lf\n", filename, threadID, numThreads, intervals);
  } else { /* Pure, untainted dark magic. */
    printf(" ");
  }
  FILE *fp = fopen(filename, "w+");
  fprintf(fp, "%d %d %32.30Lf\n", threadID, numThreads, intervals);
  fclose(fp);
}

void __init(int(*start_routine) (int, char**), int argc, char **argv) {
  memset(kill_processor, false, sizeof kill_processor);
  (*start_routine)(argc, argv);
}

int __run_processor(int pid) {
  if (!pid) {
    int i;
    for (i = 1; i <= MAXIMUM_NO_THREADS; ++i) {
      kill_processor[i] = true;
      set_core(true, i);
    }
    return 0;
  }
  if (!kill_processor[pid]) {
    (*(processor_args[pid].start_routine))(processor_args[pid].arg);
    set_core(false, pid);
  }
  return !kill_processor[pid];
}

/* pthread functions */

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  void *(*start_routine) (void *), void *arg) {
  int i;

  pthread_mutex_lock(&processor_mutex);
  /* Core 0 is busy with main */
  for (i = 1; i <= MAXIMUM_NO_THREADS; ++i) {
    if (!is_core_on(i)) {
      processor_args[i].start_routine = start_routine;
      processor_args[i].arg = arg;
      *thread = i;
      set_core(true, i);
      break;
    }
  }
  pthread_mutex_unlock(&processor_mutex);

  /* if i == 1 + MAXIMUM_NO_THREADS, all cores are busy */
  return i != 1 + MAXIMUM_NO_THREADS;
}

int pthread_join(pthread_t thread, void **retval) {
  while(is_core_on(thread));
  return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex,
  const pthread_mutexattr_t *attr) {
  *mutex = 0;
  return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
  while (load_linked(mutex) || !store_conditional(mutex, 1)) {
    // int i;
    // for (i = 0; i < 1000; ++i);
  }
  return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  *mutex = 0;
  return 0;
}

#endif
