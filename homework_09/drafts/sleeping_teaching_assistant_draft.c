#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int NUMBER_OF_SEATS = 3; // indiciates maximum number of seats

sem_t *students; // tracks students in queue
sem_t *ta; // tracks TA
sem_t *mutex; // protects access to global variables

void *ta_thread(void *param);
void *student_thread(void *param);

void *ta_loop(void *param) {
	int serving;

	
}

int main(int argc, char *argv[]){

	// init();

