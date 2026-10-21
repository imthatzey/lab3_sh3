/* SFWRENG/CS 3SH3 - Lab 3, Part I: Banking System Problem
 * Compile: gcc PLmutex.c -o PLmutex -pthread
 * Run:     ./PLmutex 100 50
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 6   /* 3 deposit + 3 withdraw */

int amount = 0;          /* shared data */
pthread_mutex_t mutex;   /* protects amount */

void *deposit(void *param);
void *withdraw(void *param);

int main(int argc, char *argv[])
{
    pthread_t tid[NUM_THREADS];   /* thread identifiers */
    pthread_attr_t attr;          /* set of thread attributes */

    /* TODO: check argc == 3, print usage message and exit if not */

    /* TODO: initialize the mutex with pthread_mutex_init(&mutex, NULL)
     *       print an error message if it does not return 0 */

    /* set the default attributes of the threads */
    pthread_attr_init(&attr);

    /* TODO: create 3 threads that run deposit(), passing argv[1]
     *   for (i = 0; i < 3; i++)
     *       pthread_create(&tid[i], &attr, deposit, argv[1]);
     *   print an error message if pthread_create fails */

    /* TODO: create 3 threads that run withdraw(), passing argv[2]
     *   use tid[3], tid[4], tid[5]
     *   print an error message if pthread_create fails */

    /* TODO: wait for all 6 threads to finish
     *   for (i = 0; i < NUM_THREADS; i++)
     *       pthread_join(tid[i], NULL); */

    /* TODO: destroy the mutex with pthread_mutex_destroy(&mutex) */

    /* TODO: print the final amount, e.g. printf("Final amount = %d\n", amount); */

    return 0;
}

/* Wife: deposits into the shared account */
void *deposit(void *param)
{
    int value = atoi(param);

    /* TODO: pthread_mutex_lock(&mutex)       (print error if it fails) */

    /* critical section */
    /* TODO: amount += value;
     *       print "Deposit amount = %d\n" with the new amount */

    /* TODO: pthread_mutex_unlock(&mutex)     (print error if it fails) */

    pthread_exit(0);
}

/* Husband: withdraws from the shared account */
void *withdraw(void *param)
{
    int value = atoi(param);

    /* TODO: pthread_mutex_lock(&mutex)       (print error if it fails) */

    /* critical section */
    /* TODO: amount -= value;
     *       print "Withdrawal amount = %d\n" with the new amount */

    /* TODO: pthread_mutex_unlock(&mutex)     (print error if it fails) */

    pthread_exit(0);
}