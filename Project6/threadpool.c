#include "threadpool.h"

void threadLoop(void* poolLoc)
{
        threadpool_t* pool = (threadpool_t*)poolLoc;
        pq_t* queue = pool->queue;

        while(1)
        {
                task_t* item = (task_t*)pq_next(queue);
                void (*function)(void* arg) = item->func;
                void* arg = item->arg;

                function(arg);
        }
}

threadpool_t* tp_create(int numThreads)
{
	threadpool_t* pool = (threadpool_t*)malloc(POOLSIZE);

	if(numThreads < 1)
		numThreads = 1;

	if(numThreads > MAXTHREADS)
		numThreads = MAXTHREADS;

	pool->numThreads = numThreads;
	pool->queue = pq_create();

	//pthread_t ids[MAXTHREADS];

	for(int i = 0; i < numThreads; i++)
	{
		pthread_t currID;
		int error = pthread_create(&currID, NULL, (void*) threadLoop, (void*)pool);

		if(error != 0)
		{
			printf("pthread_create error: %d\n", error);
			return NULL;
		}

		pool->threadIDs[i] = currID;
		//ids[i] = currID;
	}

	return pool;
}

void tp_submit(threadpool_t* tp, void* func, void* arg, short prio)
{
	pq_t* queue = tp->queue;

	task_t* job = (task_t*)malloc(TASKSIZE);
	job->func = func;
	job->arg = arg;

	pq_insert(queue, job, prio);
}
