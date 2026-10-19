#include "buffer.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <fcntl.h>

/* the buffer */

buffer_item buffer[BUFFER_SIZE];
int in = 0; // index where next item is inserted
int out = 0; // index where next item is removed

sem_t *empty; // tracks available buffer slots
sem_t *full; // tracks available items for consumers
sem_t *mutex; // protects shared buffer

int initialize_buffer(void);
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
// thread declarations
void *producer(void *param);
void *consumer(void *param);

int initialize_buffer(void) {
	empty = sem_open("empty", O_CREAT,0600, BUFFER_SIZE);
	full = sem_open("full", O_CREAT, 0600, 0);
	mutex = sem_open("mutex", O_CREAT, 0600, 1);

	sem_unlink("empty");
	sem_unlink("full");
	sem_unlink("mutex");

	return 0;
}
	
int insert_item(buffer_item item) {
	
	if(sem_wait(empty) == -1) return -1; // wait for empty slot
	
	if(sem_wait(mutex) == -1) { // enter critical section
		sem_post(empty); // return slot by incrementing
		return -1;
	}

	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;	

	if(sem_post(mutex) == -1) return -1; // leave critical section

	if(sem_post(full) == -1) return -1; // available for consumer
	return 0; // success
}

int remove_item(buffer_item *item) {
	
	if(sem_wait(full) == -1) return -1; // wait for available item

        if(sem_wait(mutex) == -1) { // enter critical section
                sem_post(full); // return slot by incrementing
                return -1;
        }

        *item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        if(sem_post(mutex) == -1) return -1; // leave critical section

        if(sem_post(empty) == -1) return -1; // make slot avail to producers
        return 0; // success

}

void *producer(void *param) {
	buffer_item item;
	(void)param;

	while(1) {
		sleep(1 + rand() % 3); // sleep for 1-3 seconds
	
		item = rand();

		if(insert_item(item) == -1)
			fprintf(stderr, "Producer: insertion error \n");
		else
			printf("Producer produced %d\n", item);
	}
	return NULL;
}

void *consumer(void *param) {
	buffer_item item;
	(void)param;

	while(1) {

		sleep(1 + rand() % 3); // sleep for 1-3 seconds
                
                if(remove_item(&item) == -1)
                        fprintf(stderr, "Consumer: removal error \n");
                else    
                        printf("Consumer consumed  %d\n", item);
	}
	return NULL;
}

int main(int argc, char *argv[]){

	// Initialize semaphores
	initialize_buffer();

	// Command line args
	int seconds = atoi(argv[1]);
	int producer_threads = atoi(argv[2]);
	int consumer_threads = atoi(argv[3]);

	pthread_t producers[producer_threads];
	pthread_t consumers[consumer_threads];

	for (int i = 0; i < producer_threads; i++) {
		pthread_create(&producers[i], NULL, producer, NULL);
	}

	for (int i = 0; i < consumer_threads; i++) {
		pthread_create(&consumers[i], NULL, consumer, NULL);
	}

	sleep(seconds);

	(void)argc; // to silence the warning
	
	return 0;
}
