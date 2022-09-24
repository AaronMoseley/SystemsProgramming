#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "proj5.h"

void setup_handler(int sig);
void await_event(void);

extern sig_atomic_t eventdone;

int main(int argc, char *argv[])
{	
	//Checks input validity
	if(argc != 2)
	{
		printf("Invalid Input\n");
		return 0;
	}

	//Logs the number of requested child processes based on the ascii value
	int numForks = *argv[1] - 48;

	if(numForks < MIN_PROC || numForks > MAX_PROC)
	{
		printf("Invalid Input\n");
		return 0;
	}

	srand(time(NULL));

	//Initial printing
	int pid = getpid();
	printf("Parent's PID: %d\n", pid);
	printf("Forking %d children...\n", numForks);

	setup_handler(SIGUSR1);

	int pids[numForks];

	int pipefd[2];
	int piped = pipe(pipefd);
	if(piped == -1)
	{
		printf("Pipe error\n");
		exit(-1);
	}

	char* writefd = (char*)malloc(sizeof(char) * 20);
	strcpy(writefd, "WRITEFD=");
	char buf[5];
	sprintf(buf, "%d", pipefd[1]);
	strcat(writefd, buf);

	char* envp[2];
	envp[0] = writefd;
	envp[1] = NULL;

	for(int i = 0; i < numForks; i++)
	{
		//Logs next PID based on parent PID
		pids[i] = pid + i + 1;	
		
		int forked = fork();
		
		fflush(stdout);	
		if(forked == 0)
		{
			close(pipefd[0]);
			
			//Arguments, sets max size of each argument string
			char *arguments[4];
			
			arguments[0] = (char*)malloc(sizeof(char) * 20);
			arguments[1] = (char*)malloc(sizeof(char) * 20);
			arguments[2] = (char*)malloc(sizeof(char) * 20);

			strcpy(arguments[0], PROGNAME);
			arguments[3] = NULL;	
			
			sprintf(arguments[1], "%d", i);
		
			if(i > 0)
			{
				sprintf(arguments[2], "%d", pids[i - 1]);
			} else {
				arguments[2] = "0";
			}
			//End of arguments

			int isError = execve(PROGNAME, arguments, envp);
			if(isError == -1)
			{
				printf("execve error\n");
				exit(-1);
			}
		} else if(forked == -1)
		{
			printf("fork error\n");
			exit(-1);
		}
	}

	int closed = close(pipefd[1]);
	if(closed == -1)
	{
		printf("Close error\n");
		exit(-1);
	}

	int numFound = 0;
	while(numFound < numForks)
	{
		int nextPID;
		if(read(pipefd[0], &nextPID, sizeof(int)) == sizeof(int))
		{
			short found = 0;
			for(int i = 0; i < numForks; i++)
			{
				if(pids[i] == nextPID)
				{
					found = 1;
				}
			}

			if(found)
			numFound++;
		}
	}

	kill(pids[numForks - 1], SIGUSR1);

	while(!eventdone)
		await_event();

	int status;
	for(int i = 0; i < numForks; i++)
	{
		//Waits and gets the child PID
		pid_t childPID = wait(&status);

		//Checks if the process exited correctly
		if(childPID > 0 && WIFEXITED(status))
		{
			int exitStatus = WEXITSTATUS(status);
			
			if(exitStatus == 255)
			{
				printf("Process %d: exited with an error, status = %d\n", childPID, exitStatus);
			} else if(exitStatus <= 200 && exitStatus >= 100)
			{
				//Logs the exit of the process
				printf("Process %d: ended normally, with status %d\n", childPID, exitStatus);
			} else {
				printf("Process %d: Something caused an error with this process, status = %d\n", childPID, exitStatus);
			}
		}
	}

	return 0;
}
