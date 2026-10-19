#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	pid_t pid, pid1, pid2;

	pid = fork();

	if (pid < 0)
	{
	printf("Something bad happened\n");
	}
	else if (pid == 0) 
	{
	pid1 = getpid();
	printf("I'm a child. My PID is %d\n", pid1); 
	char *args[] = {"./greetings.sh", NULL};
	execv("./greetings.sh", args);
	printf("Something bad happened\n");
	}
	else
	{
	pid2 = getpid();
	printf("I'm the parent. My PID is %d\n", pid2);
	wait(NULL);
	char *args[] = {"./goodbye.sh", NULL};
	execv("./goodbye.sh", args);
	printf("Something bad happened\n");	
	}
	return 0;
}
