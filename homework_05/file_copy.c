#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) // input.txt and copy.txt
{
	if(argc != 3) {
		printf("%s requires source_file destination_file\n", argv[0]);
		return EXIT_FAILURE;
	}

	int sends_file[2];
	pid_t pid;

	if(pipe(sends_file) < 0) {
		printf("There was an error with sends_file pipe");
		return EXIT_FAILURE;
	}

	pid = fork();

	if(pid < 0) {
		perror("The fork failed");
		close(sends_file[0]);
		close(sends_file[1]);
		return EXIT_FAILURE;

	} else if (pid == 0){
		close(sends_file[1]);
		int destination_fd;
		destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if(destination_fd == -1) {
			perror("could not open destination file");
			return EXIT_FAILURE;
		}

		char buffer[4096]; // buffer size for destination copy
		ssize_t bytes_read;

		while((bytes_read = read(sends_file[0], buffer, sizeof(buffer))) > 0) {
			if(write(destination_fd, buffer, bytes_read) == -1) {
				perror("writing to destination failed");
				return EXIT_FAILURE;
			}
		}

		if(bytes_read == -1) {
			perror("reading from pipe failed");
			return EXIT_FAILURE;
		}

		close(sends_file[0]);
		close(destination_fd);
	} else {
		close(sends_file[0]);
		int source_fd;
		source_fd = open(argv[1], O_RDONLY);
		if(source_fd == -1){
			perror("could not open source file");
			return EXIT_FAILURE;
		}
		char buffer[4096]; // buffer size for data transfer
		ssize_t bytes_read;

		while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
			if(write(sends_file[1], buffer, bytes_read) == -1){ // sends only bytes_read
				perror("writing to pipe failed");
				return EXIT_FAILURE;
			}
		}

		if(bytes_read == -1){
			perror("Error ocurred in reading");
			return EXIT_FAILURE;
		}

		close(source_fd);
		close(sends_file[1]);
		wait(NULL);		
		printf("%s was successfully copied to %s\n", argv[1], argv[2]);
		}
	return EXIT_SUCCESS;
}


