#ifndef MYPTHREAD_H
#define MYPTHREAD_H

/* includes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/* macros*/

#define MAXIMUM_NO_THREADS 8
#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242981
#define ADDRESS_CORE_ZERO_WRITE 115243080

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/* typedefs */

typedef char bool;
typedef int pthread_t;
typedef int pthread_mutex_t;

typedef struct {
} pthread_attr_t;

typedef struct {
} pthread_mutexattr_t;

typedef struct {
   void *(*start_routine) (void *);
   void *arg;
} processor_args_;

/* global variables */

int processor_number = 0;

bool kill_processor[MAXIMUM_NO_THREADS];
const bool true = 1;
const bool false = 0;

pthread_mutex_t processor_mutex = 0;

processor_args_ processor_args[MAXIMUM_NO_THREADS];

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

void __init(int(*start_routine) (int, char**), int argc, char **argv) {
  memset(kill_processor, false, sizeof kill_processor);
  (*start_routine)(argc, argv);
}

int __run_processor(int pid) {
  if (!pid) {
    int i;
    for (i = 1; i < MAXIMUM_NO_THREADS; ++i) {
      kill_processor[i] = true;
      set_core(true, i);
    }
    return 0;
  }
  (*(processor_args[pid].start_routine))(processor_args[pid].arg);
  set_core(false, pid);
  return kill_processor[pid];
}

/* pthread functions */

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  void *(*start_routine) (void *), void *arg) {
  int i;

  pthread_mutex_lock(&processor_mutex);
  for (i = 0; i < MAXIMUM_NO_THREADS; ++i) {
    if (!is_core_on(i)) {
      processor_args[i].start_routine = start_routine;
      processor_args[i].arg = arg;
      *thread = i;
      set_core(true, i);
    }
  }
  pthread_mutex_unlock(&processor_mutex);

  /* if i == MAXIMUM_NO_THREADS, all cores are busy */
  return i != MAXIMUM_NO_THREADS;
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
  while (load_linked(mutex) || !store_conditional(mutex, 1));
  return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
  *mutex = 0;
  return 0;
}

#endif
