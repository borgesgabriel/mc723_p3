#ifndef MYPTHREAD_H
#define MYPTHREAD_H

/* typedefs */

typedef unsigned long int pthread_t;

typedef struct {

} pthread_mutex_t;

typedef struct {

} pthread_attr_t;

typedef struct {

} pthread_mutexattr_t;

/* pthread functions */

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg) {

}

int pthread_join(pthread_t thread, void **retval) {

}

int pthread_mutex_init(pthread_mutex_t *mutex, 
    const pthread_mutexattr_t *attr) {

}

int pthread_mutex_lock(pthread_mutex_t *mutex) {

}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {

}

#endif