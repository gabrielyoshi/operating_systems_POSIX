#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define NUM_CHAIRS 3
#define NUM_STUDENTS 5

// Variable Declarations
pthread_mutex_t mutex;

// Semaphore Declaration
sem_t *students; // counts waiting students not helped by TA
sem_t *student_done[NUM_STUDENTS]; // signals TA has finished helping student

// shared variables
int waiting_students = 0;
int waiting_queue[NUM_CHAIRS];
int front = 0;
int rear = 0;

// Function declarations
void *ta_function(void *arg);
void *student_function(void *arg);
int initialize(void);

int initialize(void) {
	students = sem_open("/students", O_CREAT, 0666, 0);
	
	for (int i = 0; i < NUM_STUDENTS; i++) {
		char name[32];
		snprintf(name, sizeof(name), "/student_done_%d", i);
		student_done[i] = sem_open(name, O_CREAT, 0666, 0);
		sem_unlink(name);
	}
	sem_unlink("/students");
	return 0;
}

void *ta_function(void *arg) {
	while (1) {
		// prevent unused-parameter warning
		(void)arg;

		sem_wait(students);
		pthread_mutex_lock(&mutex);

		// Critical Section
		int student_id = waiting_queue[front];
		front = (front + 1) % NUM_CHAIRS;
		waiting_students--;
		printf("TA helps student %d. Waiting students: %d\n", student_id, waiting_students);
		// End of Critical Section
		
		pthread_mutex_unlock(&mutex);
		
		printf("TA is helping student %d.\n", student_id);
		sleep((rand() % 3) + 1);\
		printf("TA finished helping student %d.\n", student_id);

		sem_post(student_done[student_id - 1]);
	}
	return NULL;
}

void *student_function(void *arg) {
	while (1) {	

		// (int *) : Casts the generic pointer to an integer pointer, 
		// *(int *) : dereference pointer
		int student_id = *(int *)arg;

		printf("Student %d is programming.\n", student_id);
		sleep((rand() % 3) + 1); // student spends time programming

		pthread_mutex_lock(&mutex);
		
		// Critical Section		
		if (waiting_students < NUM_CHAIRS) {
			waiting_queue[rear] = student_id;
			
			// advances the rear index
			rear = (rear + 1) % NUM_CHAIRS;
				
			waiting_students++;
			
			printf("Student %d enters waiting queue.\n", student_id);
			sem_post(students);
		// End of Critical Section	
			pthread_mutex_unlock(&mutex);
			sem_wait(student_done[student_id - 1]);
			printf("Student %d is back to programming.\n", student_id);
		} else {
			pthread_mutex_unlock(&mutex);
			printf("No chair available.\n");
		}
		
		}
	return NULL;
}

int main(int argc, char *argv[]) {
	// Initialize semaphore
	initialize();
	
	pthread_t ta_thread;
	pthread_t student_threads[NUM_STUDENTS];
	
	pthread_mutex_init(&mutex, NULL);	

	pthread_create(&ta_thread, NULL, ta_function, NULL);
	
	int student_ids[NUM_STUDENTS];

	for (int i = 0; i < NUM_STUDENTS; i++) {
		student_ids[i] = i + 1;
		pthread_create(&student_threads[i], NULL, student_function, &student_ids[i]);
	}
	for (int i = 0; i < NUM_STUDENTS; i++) {
		pthread_join(student_threads[i], NULL);
	}
	(void)argc; 
	(void)argv;
	return 0;
}


