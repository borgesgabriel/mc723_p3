#ifndef MYPTHREAD_HELPER_H
#define MYPTHREAD_HELPER_H

/* macros*/

#define MAXIMUM_NO_THREADS 7
#define NUMBER_OF_CORES_REQUEST 115242980
#define ADDRESS_CORE_ZERO_READ 115242984
#define ADDRESS_CORE_ZERO_WRITE 115243080
#define GET_RESULT_REQUEST 15242980
#define OFFLOAD_DEBUG 0

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

bool kill_processor[1 + MAXIMUM_NO_THREADS];
const bool true = 1;
const bool false = 0;

pthread_mutex_t processor_mutex = 0;

processor_args_ processor_args[1 + MAXIMUM_NO_THREADS];

/* helper functions */

int load_linked(int* ptr);

int store_conditional(int* ptr, int value);

void set_core(bool on, int core_num);

uint32_t number_of_cores();

bool is_core_on(int core_num);

long double read_piargs(int threadID);

void write_piargs(int threadID, double intervals, int numThreads);

void __init(int(*start_routine) (int, char**), int argc, char **argv);

int __run_processor(int pid);

/* pthread functions */

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  void *(*start_routine) (void *), void *arg);

int pthread_join(pthread_t thread, void **retval);

int pthread_mutex_init(pthread_mutex_t *mutex,
  const pthread_mutexattr_t *attr);

int pthread_mutex_lock(pthread_mutex_t *mutex);

int pthread_mutex_unlock(pthread_mutex_t *mutex);

#endif
