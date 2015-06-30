/* pi.c - parallel C code to demonstrate Linux thread interface
 * Original Source: www.tldp.org/HOWTP/Parallel_processing-HOWTO
 * Since PI == 4 * arctan(1), and arctan(x) is the
 *  integral from 0 to x of (1/(1+x*x),
 *  the for loop below approximates that integration.
 * Generalized by: Joel Adams
 * Usage: ./a.out <numIntervals> <numThreads>
 */

#include "../mypthread.h"

#define kOffload true

/* global variables (shared by all threads */
volatile long double pi = 0.0; /* the approximation, to 31 sigfigs */
pthread_mutex_t piLock;        /* how we synchronize writes to 'pi' */
long double intervals;         /* how finely we chop the integration */
int numThreads;                /* how many threads we use */
 pthread_mutex_t ah = 0;

long double offload_arctan(int threadID, long double intervals, int numThreads) {
  long double result;
  if (kOffload) {
    pthread_mutex_lock(&ah);
    write_piargs(threadID, intervals, numThreads);
    result = read_piargs(threadID);
    pthread_mutex_unlock(&ah);
  } else {
    long double localSum = 0, x, width = 1.0 / intervals;
    int i;
    for (i = threadID; i < intervals; i += numThreads) {
      x = (i + 0.5) * width;
      localSum += 4.0 / (1.0 + x*x);
    }
    result = localSum * width;
  }
  return result;
}

/* the function a thread executes
 * Parameters: arg, a void* storing the address of the thread ID.
 */
void *computePI(void *id) {
  long double localSum = offload_arctan(*((int*)id), intervals, numThreads);

  pthread_mutex_lock(&piLock);
  pi += localSum;
  pthread_mutex_unlock(&piLock);

  return NULL;
}

int pi_main(int argc, char **argv) {
    pthread_t *threads;        /* dynarray of threads */
    void *retval;              /* unused; required for join() */
    int *threadID;             /* dynarray of thread id #s */
    int i;                     /* loop control variable */

 if (argc == 3) {
  intervals = atoi(argv[1]);
  numThreads = atoi(argv[2]);

  if (numThreads < 1 || numThreads > MAXIMUM_NO_THREADS) {
    printf("Invalid number of threads. Number of threads should"
     " be between 1 and %d.\n", MAXIMUM_NO_THREADS);
    return 0;
  }

  if (kOffload && !OFFLOAD_DEBUG && numThreads > 2) {
    numThreads = 2;
  }

  threads = malloc(numThreads*sizeof(pthread_t));
  threadID = malloc(numThreads*sizeof(int));
  pthread_mutex_init(&piLock, NULL);

  for (i = 0; i < numThreads; i++) {
    threadID[i] = i;
    pthread_create(&threads[i], NULL, computePI, threadID+i);
  }

  for (i = 0; i < numThreads; i++) {
   pthread_join(threads[i], &retval);
 }
 printf("\nEstimation of pi is %32.30Lf \n", pi);
 printf("(actual pi value is 3.141592653589793238462643383279...)\n");
} else {
  printf("Usage: ./a.out <numIntervals> <numThreads>");
}

return 0;
}

int main(int argc, char **argv) {
  pthread_mutex_lock(&processor_mutex);
  int pid = processor_number++;
  pthread_mutex_unlock(&processor_mutex);

  if (!pid) {
    __init(pi_main, argc, argv);
  }

  while(__run_processor(pid));

  exit(0);
  return 0;
}
