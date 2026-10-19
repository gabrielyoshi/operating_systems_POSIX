#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
	int sends_string[2];
	int returns_string[2];
	ssize_t bytes_read;	

	pid_t pid;

	if( pipe(sends_string) < 0) {
		printf("There was an error with sends-string pipe");
	}

	if (pipe(returns_string) < 0) {
		printf("There was an error with returns_string pipe");
	}

	pid = fork();

	if(pid < 0){
		printf("Fork failed");
	}
	else if(pid == 0){
		close(sends_string[1]); 
		close(returns_string[0]);
		char received_message[100];
		bytes_read = read(sends_string[0], received_message, sizeof(received_message) - 1);
		if (bytes_read < 0) {
			printf("There was an error reading sends_string pipe");
		} else if (bytes_read == 0) {
			printf("No data in the pipe");
		} else {
			received_message[bytes_read] = '\0';
		}
		close(sends_string[0]);
		for (int i = 0; received_message[i] != '\0'; i++) {
			if (islower(received_message[i])){
				received_message[i] = toupper(received_message[i]);
			}
			else if (isupper(received_message[i])){
				received_message[i] = tolower(received_message[i]);
			}
		}
		if(write(returns_string[1], received_message, strlen(received_message) + 1) == -1){
			printf("Write error to returns_string pipe occured");
			}
		close(returns_string[1]);
		 
	} else {
		close(sends_string[0]);
		close(returns_string[1]);
		char message[] = "Hello there!";
		char change_case_message[sizeof(message)];

		if(write(sends_string[1], message, sizeof(message)) == -1){
			printf("Write error to sends_string pipe occurred");
			}
		
		bytes_read = read(returns_string[0], change_case_message, sizeof(change_case_message) - 1);
		if (bytes_read < 0) {
			printf("There was an error reading the returns_string pipe");
		} else if (bytes_read == 0) {
			printf("No data in the pipe");
		} else {
			change_case_message[bytes_read] = '\0';
			printf("%s\n", change_case_message);
			wait(NULL);
		}
		close(sends_string[1]);
		
	}
	return 0;
}
