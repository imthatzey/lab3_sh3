/* SFWRENG/CS 3SH3 - Lab 3, Part II: Banking System with Semaphores
 * Compile: gcc labp2.c -o labp2 -pthread
 * Run:     ./labp2 100
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 10   /* 7 deposit + 3 withdraw */

int amount = 0;          /* shared data */
pthread_mutex_t mutex;   /* protects amount */

void *deposit(void *param);
void *withdraw(void *param);

// declare semaphores
sem_t can_deposit;    // how many more 100s fit before amount reaches 400
sem_t can_withdraw;   // how many 100s are available to withdraw

int main(int argc, char *argv[])
{
    pthread_t tid[NUM_THREADS];   /* thread identifiers */
    pthread_attr_t attr;          /* set of thread attributes */

    if (argc != 2) {
        printf("Usage: %s <deposit/withdraw amount>\n", argv[0]);
        return 1;
    }

    if (atoi(argv[1]) != 100) { // exit if the entered value isn't 100
        printf("Amount must be 100!\n");
        return 1;
    }

    // amount starts at 0, so there is room for 4 deposits (400 / 100)
    if (sem_init(&can_deposit, 0, 4) != 0) {
        printf("Error: can_deposit semaphore not initialized\n");
        return 1;
    }

    // amount starts at 0, so nothing can be withdrawn yet
    if (sem_init(&can_withdraw, 0, 0) != 0) {
        printf("Error: can_withdraw semaphore not initialized\n");
        return 1;
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error: mutex init failed\n");
        return 1;
    }

    pthread_attr_init(&attr);

    int i = 0;

    for (i = 0; i < 7; i++) { // create 7 deposit threads
        if (pthread_create(&tid[i], &attr, deposit, argv[1]) != 0) {
            printf("Error: failed to create deposit thread %d\n", i);
            return 1;
        }
    }

    for (i = 7; i < 10; i++) { // create 3 withdraw threads
        if (pthread_create(&tid[i], &attr, withdraw, argv[1]) != 0) {
            printf("Error: failed to create withdraw thread %d\n", i);
            return 1;
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&can_deposit);
    sem_destroy(&can_withdraw);

    printf("Final amount = %d\n", amount);

    return 0;
}

/* Wife: deposits into the shared account */
void *deposit(void *param)
{
    printf("Start of deposit().\n");

    int value = atoi(param);

    // wait for room (amount < 400); must come before locking the mutex
    if (sem_wait(&can_deposit) != 0) {
        printf("Error: deposit failed to wait on semaphore\n");
        pthread_exit(0);
    }

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

    // there is now 100 more to withdraw
    if (sem_post(&can_withdraw) != 0) {
        printf("Error: deposit failed to post semaphore\n");
    }

    pthread_exit(0);
}

/* Husband: withdraws from the shared account */
void *withdraw(void *param)
{
    printf("Start of withdraw().\n");

    int value = atoi(param);

    // wait for money (amount > 0); must come before locking the mutex
    if (sem_wait(&can_withdraw) != 0) {
        printf("Error: withdraw failed to wait on semaphore\n");
        pthread_exit(0);
    }

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

    // there is now room for 100 more
    if (sem_post(&can_deposit) != 0) {
        printf("Error: withdraw failed to post semaphore\n");
    }

    pthread_exit(0);
}
