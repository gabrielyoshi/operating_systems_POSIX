#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int sequence_length;
	int *sequence;
} fibonacci_data;

void *fibonacci_generator(void *arg);

int main(int argc, char *argv[]) {
	
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <count>\n", argv[0]);
		return 1;
	}
	
	// Sets up array for fibonacci sequence
	int fib_num_count = atoi(argv[1]);
	int fibonacci_sequence[fib_num_count];

	fibonacci_data data;
	data.sequence_length = fib_num_count;
	data.sequence = fibonacci_sequence;

	// Setup thread
	pthread_t thread;
	pthread_create(&thread, NULL, fibonacci_generator, &data);
	pthread_join(thread, NULL);
	
	for (int i = 0; i < fib_num_count; i++) {
		printf("%d ", fibonacci_sequence[i]);
	}
	printf("\n");
	printf("Fibonacci generator has finished.\n");

	return 0;
}

void *fibonacci_generator(void *arg) {
	// address cast defined as a fibonacci_data struct
	fibonacci_data *data = arg;

	int count = data->sequence_length;
	int *sequence = data->sequence;
	
	if (count >= 1) {
		sequence[0] = 0;
	}
	if (count >= 2) {
		sequence [1] = 1;
	}
	for (int i=2; i <= count; i++) {
		sequence[i] = sequence[i-1] + sequence[i-2];
	}
	
	return NULL;
}

