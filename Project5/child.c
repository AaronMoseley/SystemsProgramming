#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void await_event(void);
void setup_handler(int sig);

extern sig_atomic_t eventdone;

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Incorrect number of arguments\n");
		exit(-1);
	}

	setup_handler(SIGUSR1);	

	int pid = getpid();
	int writefd = atoi(getenv("WRITEFD"));

	if(write(writefd, &pid, sizeof(int)) < sizeof(int))
	{
		printf("Write error\n");
		exit(255);
	}

	int nextProc = atoi(argv[2]);

	while(!eventdone)
		await_event();

	printf("Process %s (pid %d) greets you.\n", argv[1], getpid());

	kill(nextProc, SIGUSR1);

	srandom(time(0) + atoi(argv[1]));
	int exitStatus = (random() % 100) + 100;

	exit(exitStatus);
}
