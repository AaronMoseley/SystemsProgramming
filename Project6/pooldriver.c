#include <stdio.h>
#include <stdlib.h> // exit(), etc.
#include <unistd.h> // write() - sheesh
#include <signal.h> // for sigaction
#include "threadpool.h"

#define SEED 0xdeadbeef  // for repeatability
#define MAXSLEEP 5
#define MAXLINE 128
#define MAXTASKS 200

/* a structure to hold the arguments to my jobs */
typedef struct {
  short secs;
  short prio;
  int seqno;
} arg_t;

/* a trivial job to be executed */
void task1(void *a) {
  arg_t *arg = (arg_t *)a;
  printf("----task %d, prio %d, sleeping %d.\n",arg->seqno,
	 arg->prio,arg->secs);
  sleep(arg->secs);
  printf("^^^^task %d is finished.\n",arg->seqno);
}

/* wait for a signal to stop */
void handler(int sig) {  // 
  puts("======Caught SIGALARM, exiting.");
  fflush(stdout);
}

int main(int argc, char *argv[]) {
  int i;
  int totaltime = 0;
  struct sigaction myaction;
  if (argc != 3) {
    fprintf(stderr,"Usage: %s <#threads> <#tasks>\n",argv[0]);
    exit(1);
  }
  int size = atoi(argv[1]);
  int ntasks = atoi(argv[2]);
  if (ntasks <= 0 || ntasks > MAXTASKS) {
    fprintf(stderr,"#tasks must be positive and at most %d\n",MAXTASKS);
    exit(10);
  }
  sigfillset(&myaction.sa_mask);
  myaction.sa_handler = handler;
  myaction.sa_flags = 0;
  // NOTE use of sigaction instead of signal()!
  if (sigaction(SIGALRM,&myaction,NULL) <0) {
    perror("sigaction");
    exit(2);
  }
  srandom(SEED);
  arg_t *params = calloc(ntasks,sizeof(arg_t));
  threadpool_t *pool = tp_create(size);
  for (i=0; i<ntasks; i++) {
    params[i].secs = (random() % MAXSLEEP) + 1;
    params[i].prio = random() & 0xFFFF;
    params[i].seqno = i;
    totaltime += params[i].secs;
  }
  // XXX DEBUG
  //  printf("total sleep time = %d seconds.\n",totaltime);
  for (i=0; i<ntasks; i++)
    tp_submit(pool,task1,&params[i],params[i].prio);
  printf("===================submitted %d tasks.\n",ntasks);
  alarm(MAXSLEEP*ntasks/size);  //  uipper bound on sleep time
  pause();  // we can use pause() - no race
  exit(0);
}
