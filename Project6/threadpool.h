#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pq.h"

#ifndef THREADPOOL_H
#define THREADPOOL_H

#define POOLSIZE 256
#define TASKSIZE 16

#define MAXTHREADS 12   // arbitrary limit

typedef struct task_t{
	void* func;
	void* arg;
} task_t;

typedef struct threadpool_t{
	int numThreads;
	pq_t* queue;
	pthread_t threadIDs[12];	 
} threadpool_t;

/* Create and return a pointer to an instance of the type
 * declared above.
 */
threadpool_t *tp_create(int nthreads);

/* Submit a job (task) to be executed by the given thread pool.
 * Jobs are functions that will be executed in a pool thread,
 * and passed the given parameter (arg).  Jobs are executed
 * in priority order, with higher priorities executed first.
 * Starvation of a low-priority job is possible if jobs with
 * higher priorities are submitted continually.
 */
void tp_submit(threadpool_t *tp, void* func, void* arg, short prio);
#endif
