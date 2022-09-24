#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "forker.h"

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
	printf("Forker Process ID = %d\n", getpid());

	printf("Forking %d children...\n", numForks);

	//Sets cookie as random string I created
	char cookie[MAX_COOKIE] = "ac7ctuqk62e1";
	
	//Logs pid for later use
	int pid = getpid();

	int pids[numForks];

	for(int i = 0; i < numForks; i++)
	{
		//Logs next PID based on parent PID
		pids[i] = pid + i + 1;	
		
		int forked = fork();
		
		fflush(stdout);	
		if(forked == 0)
		{
			//Changes cookie
			int randomPos = (rand() % (12 - numForks)) + i;
			cookie[randomPos] += 1;

			printf("Child %d is process %d. Cookie is %s.\n", i, pids[i], cookie);

			//Arguments, sets max size of each argument string
			char *arguments[i + 3];
			for(int j = 0; j < i + 3; j++)
			{
				arguments[j] = (char*)malloc(sizeof(char) * 20);
			}

			strcpy(arguments[0], PROGNAME);
			arguments[i + 2] = NULL;	
			
			sprintf(arguments[1], "%d", i);
		
			for(int j = 0; j < i; j++)
			{
				sprintf(arguments[j + 2], "%d", pids[j]);
			}
			//End of arguments

			//Environment, sets max size of each environment string	
			char *environment[2];
			environment[0] = (char*)malloc(sizeof(char) * 20);
			environment[1] = NULL;
			
			strcpy(environment[0], "COOKIE=");
			strcat(environment[0], cookie);
			//End of environment
			
			int isError = execve(PROGNAME, arguments, environment);
			if(isError == -1)
			{
				printf("execve error: %d\n", errno);
				exit(-1);
			}
		} else if(forked == -1)
		{
			printf("fork error: %d\n", errno);
			exit(-1);
		}
	}

	int status;
	for(int i = 0; i < numForks; i++)
	{
		//Waits and gets the child PID
		pid_t childPID = wait(&status);

		//Checks if the process exited correctly
		if(childPID > 0 && WIFEXITED(status))
		{
			//Logs the exit of the process
			printf("Process %d: ended normally, with status %d\n", childPID, WEXITSTATUS(status));
		}
	}

	return 0;
}
