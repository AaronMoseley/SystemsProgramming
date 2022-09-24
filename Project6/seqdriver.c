/* Simple test driver to verify correct sorting of the queue. */
/* SINGLE-THREADED - does not stress the synch mechanisms.    */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include "pq.h"
#include <malloc.h> // for malloc()
#include <limits.h> // for SHORT_MIN
#include <string.h> // for perror
#include <errno.h>
#include <assert.h>

/* Modify these to experiment with different parameters. */
#define NITEMS 100
#define EQ_PRIO 30

typedef struct {
  int seq;
  short prio;
} item_t;


int main(int argc, char *argv[]) {	
  int i;
  item_t items[NITEMS];
  pq_t *queue;

  // create a shared instance of the priority queue
  queue = pq_create();
  // create items to be inserted
  for (i=0; i<EQ_PRIO; i++) {  // first, some with priority 0
    items[i].prio = 0;
    items[i].seq = i;
    pq_insert(queue,&items[i],0);
  }

  for (; i < NITEMS; i++) {
    short s;
    if (getentropy(&s,sizeof(s)) < 0) {
      perror("getentropy");
      exit(3);
    }
    items[i].prio = s;
    items[i].seq = i;
    pq_insert(queue,&items[i],s);
  }

  // now get and print the whole list
  for (i=0; i<NITEMS + 1; i++) {
    item_t *item = (item_t *)pq_next(queue);  // should not block!
    printf("item %d: prio = %d, seq = %d\n",i,item->prio,item->seq);
  }
  exit(0);
}
