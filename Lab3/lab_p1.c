/* SFWRENG/CS 3SH3 - Lab 3, Part I: Banking System Problem
 * Compile: gcc lab_p1.c -o lab3 -pthreads
 * Run:     ./lab3 100 50
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

    if (argc != 3) {
        printf("Usage: %s <deposit amount> <withdraw amount>\n", argv[0]);
        return 1;
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error: mutex init failed\n");
        return 1;
    }

    pthread_attr_init(&attr);

    int i = 0;

    for (i = 0; i < 3; i++) {
        if (pthread_create(&tid[i], &attr, deposit, argv[1]) != 0) {
            printf("Error: failed to create deposit thread %d\n", i);
            return 1;
        }
    }

    for (i = 3; i < 6; i++) {
        if (pthread_create(&tid[i], &attr, withdraw, argv[2]) != 0) {
            printf("Error: failed to create withdraw thread %d\n", i);
            return 1;
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Final amount = %d\n", amount); 

    return 0;
}

/* Wife: deposits into the shared account */
void *deposit(void *param)
{
    int value = atoi(param);

    if (pthread_mutex_lock(&mutex) != 0) {
        printf("Error: deposit failed to lock mutex\n");
        pthread_exit(0);
    }

    /* critical section */
    amount += value;
    printf("Deposit amount = %d\n", amount);

    if (pthread_mutex_unlock(&mutex) != 0) {
        printf("Error: deposit failed to unlock mutex\n");
    }

    pthread_exit(0);
}

/* Husband: withdraws from the shared account */
void *withdraw(void *param)
{
    int value = atoi(param);

    if (pthread_mutex_lock(&mutex) != 0) {
        printf("Error: withdraw failed to lock mutex\n");
        pthread_exit(0);
    }

    /* critical section */
    amount -= value;
    printf("Withdrawal amount = %d\n", amount);

    if (pthread_mutex_unlock(&mutex) != 0) {
        printf("Error: withdraw failed to unlock mutex\n");
    }

    pthread_exit(0);
}
