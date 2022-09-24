/* Simple program for "forker" to fork-exec */
/* Just print the PID and arguments. */

#include <stdio.h>
#include <sys/types.h> // for pid_t
#include <unistd.h> // for pid_t
#include <stdlib.h> // for getenv(), exit()
#include "forker.h" // for COOKIE_KEY

int main(int argc, char *argv[]) {
  char *cookie;
  int i;
  int r;

  pid_t pid = getpid();
  printf("PID=%d\n",pid);
  for (i=0; i<argc; i++)
    printf("argv[%d] = %s%s\n",i,argv[i],(i < argc-1 ? " " : "" ) );

  cookie = getenv(COOKIE_KEY);
  printf("value of " COOKIE_KEY " = %s\n",cookie);
  srandom(pid); // seed the RNG
  r = (long) (random() % 200) + 50; // get a random # in [0,250)
  printf("Exiting with status %d.\n",r);
  exit(r);
}

  
  
