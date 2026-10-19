#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/**

* Header file for dining philosophers

*/

// the number of philosophers

#define NUMBER          5

// the maximum time (in seconds) to sleep

#define MAX_SLEEP_TIME  5

// different philosopher states

//#define THINKING              0

//#define HUNGRY                        1

//#define EATING                        2

// the state of each philosopher (THINKING, HUNGRY, EATING)

//int state[NUMBER];

enum {THINKING, HUNGRY, EATING} state[NUMBER];

// the thread id of each philosopher (0 .. NUMBER - 1)

int thread_id[NUMBER];

// condition variables and associated mutex lock
pthread_cond_t          cond_vars[NUMBER];
pthread_mutex_t         mutex_lock;

//void *philosopher(void *param);
/* return the left neighbor */

int left_neighbor(int number)

{
        if (number == 0)
                return NUMBER - 1;
        else
                return number - 1;
}

/* return the right neighbor */

int right_neighbor(int number)

{
        if (number == NUMBER - 1)
                return 0;
        else
                return number + 1;
}
void test(int i)
{
        /**
         * Basic idea is this:
         * If I'm hungry, and my left and right neighbors aren't eating, then let me eat.
         */

        if ( (state[left_neighbor(i)] != EATING) && (state[i] == HUNGRY) && (state[right_neighbor(i)] != EATING) ) {

                state[i] = EATING;

                pthread_cond_signal(&cond_vars[i]);
        }
} 

/**

* A philosopher calls this when it

* wants to pick up its forks.

*/

void pickup_forks(int number)

{
        pthread_mutex_lock(&mutex_lock);

        state[number] = HUNGRY;

        test(number);

        while (state[number] != EATING) {
                pthread_cond_wait(&cond_vars[number], &mutex_lock);
        }
        pthread_mutex_unlock(&mutex_lock);
}
/**

* A philosopher calls this when it

* wants to return its forks.

*/
void return_forks(int number)
{
        pthread_mutex_lock(&mutex_lock);

        state[number] = THINKING;

        test(left_neighbor(number));

        test(right_neighbor(number));

        pthread_mutex_unlock(&mutex_lock);
}

void eating(int sleep_time)
{
        //printf("eating for %d seconds\n",sleep_time);

        sleep(sleep_time);
}

void thinking(int sleep_time)
{
        //printf("thinking for %d seconds",sleep_time);
        sleep(sleep_time);
}

void *philosopher(void *param)
{
        int *lnumber = (int *)param;

        int number = *lnumber;

        int sleep_time;

        int times_through_loop = 0;

       srandom((unsigned)time(NULL));

        while (times_through_loop < 5) {

                sleep_time = (int)((random() % MAX_SLEEP_TIME) + 1);

                thinking(sleep_time);

                pickup_forks(number); 

                printf("Philosopher %d is eating\n",number);

                printf(" left is ");

                if (state[left_neighbor(number)] == THINKING)

                    printf("THINKING");

                else if (state[left_neighbor(number)] == HUNGRY)

                    printf("HUNGRY");

                else

                    printf("EATING");

                printf(" and right is ");

                if (state[right_neighbor(number)] == THINKING)

                    printf("THINKING");

                else if (state[right_neighbor(number)] == HUNGRY)

                    printf("HUNGRY");

                else

                    printf("EATING");

                sleep_time = (int)((random() % MAX_SLEEP_TIME) + 1);

                eating(sleep_time);

                printf("Philosopher %d is thinking\n",number);

                return_forks(number);

                ++times_through_loop;
        }

return lnumber;

}

pthread_t tid[NUMBER];

void init()
{
int i;

        for (i = 0; i < NUMBER; i++) {
                state[i] = THINKING;
                thread_id[i] = i;
                pthread_cond_init(&cond_vars[i],NULL);
        }
        pthread_mutex_init(&mutex_lock, NULL);
}

void create_philosophers()

{
int i;

        for (i = 0; i < NUMBER; i++) {
                pthread_create(&tid[i], 0, philosopher, (void *)&thread_id[i]);
        }
}

int main(void)
{
int i;

        init();

        create_philosophers();

        for (i = 0; i < NUMBER; i++)
                pthread_join(tid[i],NULL);
        return 0;
}
