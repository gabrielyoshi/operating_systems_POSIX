#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/wait.h>

#include <sys/time.h>

#include <sys/stat.h>

#include <unistd.h>

int main(int argc, char *argv[])
{

	struct timeval *start;
	struct timeval end;
	pid_t pid;
	int fd[2];

	if(argc == 1)
		return 0;

	if(pipe(fd) == -1) {
		printf("Pipe failed\n");

	return -1;
	}

	pid = fork();

	if(pid == 0)
	{
		//close read end
		close(fd[0]);

		start = (struct timeval *)malloc(sizeof(struct timeval));

		// child
		if (gettimeofday(start, NULL) != 0)
			return 0;

		// write the starting time to pipe
		write(fd[1], start, sizeof(struct timeval));

		execvp(argv[1],&argv[1]);
		fprintf(stderr, "Error");
	}

	else {
		wait(NULL);

		if (gettimeofday(&end, NULL) != 0)

            		return -1;

 

        close(fd[1]);

 

        // read the starting time from shared memory

        int start_seconds = 0;

        int start_useconds = 0;

 

        start = (struct timeval *)malloc(sizeof(struct timeval));

 

        read(fd[0],start, sizeof(struct timeval));

        start_seconds = start->tv_sec;

        start_useconds = start->tv_usec;

 

        // now determine elapsed time

        int seconds = (end.tv_sec - start_seconds);

        int useconds = (end.tv_usec - start_useconds);

        if (useconds < 0) {

           seconds--;

           useconds += 1000000;
 	}

        printf("Elapsed time %d.%06d\n",seconds,useconds);

    }

    return 0;

}
