#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// Synchoronization
pthread_mutex_t mutex;
pthread_cond_t condition[NUM_PHILOSOPHERS];

// Philosopher states
typedef enum {
	THINKING,
	HUNGRY,
	EATING
} PhilosopherState;

const char *state_names[] = {
	"THINKING",
	"HUNGRY",
	"EATING"
};

// Shared variable
// an array of philosopher states
PhilosopherState state[NUM_PHILOSOPHERS];

// Function declarations
void initialize(void);
void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);
void *philosopher(void *arg);

void initialize(void) {
	pthread_mutex_init(&mutex, NULL);

	for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
		pthread_cond_init(&condition[i], NULL);

		state[i] = THINKING;
	}
}

void pickup_forks(int philosopher_number) {
	pthread_mutex_lock(&mutex);
	state[philosopher_number] = HUNGRY;
	printf("Philosopher %d is %s.\n", philosopher_number, 
		state_names[state[philosopher_number]]);

	int left = (philosopher_number + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
	int right = (philosopher_number + NUM_PHILOSOPHERS + 1) % NUM_PHILOSOPHERS;
	while (state[left] == EATING || state[right] == EATING) {
		printf("Philosopher %d must wait.\n", philosopher_number);
		pthread_cond_wait(&condition[philosopher_number], &mutex);
	}
	state[philosopher_number] = EATING;
	pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number) {
	pthread_mutex_lock(&mutex);
	state[philosopher_number] = THINKING;

	printf("Philosopher %d is now  %s.\n", philosopher_number, 
                state_names[state[philosopher_number]]);

	int left = (philosopher_number + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
        int right = (philosopher_number + NUM_PHILOSOPHERS + 1) % NUM_PHILOSOPHERS;
	pthread_cond_signal(&condition[left]);
	printf("Philosopher %d returns left fork and nods to philosopher %d.\n", 
		philosopher_number, left);
	pthread_cond_signal(&condition[right]);
	printf("Philosopher %d returns right fork and nods to philosopher %d.\n", 
		philosopher_number, right);
	pthread_mutex_unlock(&mutex);
}

void *philosopher(void *arg) {
	int philosopher_number = *(int *)arg;

	while (1) {
		printf("Philosopher %d is thinking.\n", philosopher_number);

		sleep((rand() % 3) + 1);

		pickup_forks(philosopher_number);

		printf("Philosopher %d is eating.\n", philosopher_number);

		sleep((rand() % 3) + 1);

		return_forks(philosopher_number);
	}
	return NULL;
}

int main(void) {
	initialize();

	pthread_t philosopher_threads[NUM_PHILOSOPHERS];

	int philosopher_ids[NUM_PHILOSOPHERS];

	for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
		philosopher_ids[i] = i;

		pthread_create(&philosopher_threads[i], 
			NULL, philosopher, &philosopher_ids[i]);
	}

	for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
		pthread_join(philosopher_threads[i], NULL);
	}

	return 0;
}

