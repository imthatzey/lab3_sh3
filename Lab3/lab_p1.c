/* SFWRENG/CS 3SH3 - Lab 3, Part I: Banking System Problem
 * Compile: gcc lab_p1.c -o lab3 -pthreads
 * Run:     ./lab3 100 50
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

// declare semaphore
sem_t sem;

int main(int argc, char *argv[])
{
    // initialize semaphore
    if (sem_init(&sem, 0, 1) != 0){
        printf("Error: semaphore not initialized\n");
    }

    pthread_t tid[NUM_THREADS];   /* thread identifiers */
    pthread_attr_t attr;          /* set of thread attributes */

    if (argc != 2) {
        printf("Usage: %s <deposit/withdraw amount>\n", argv[0]);
        return 1;
    }
    
    if (atoi(argv[1]) != 100){ // exit if the entered value isn't 100
        printf("Amount must be 100!\n");
        return 1;
    }
    
    argv[2] = argv[1]; // since both withdraw and deposit are 100, copy the value
    
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
    printf("Start of deposit().\n");
    
    int value = atoi(param);

    if (pthread_mutex_lock(&mutex) != 0) {
        printf("Error: deposit failed to lock mutex\n");
        pthread_exit(0);
    }
 
    //acquire semaphore
    sem_wait(&sem);

    /* critical section */
    if (amount < 400){ // only allow deposits if amount is less than 400
        amount += value;
        printf("Deposit amount = %d\n", amount);
    } else {
        printf("Couldn't deposit; too many savings.\n");
    }
    
    // release semaphore
    sem_post(&sem);

    if (pthread_mutex_unlock(&mutex) != 0) {
        printf("Error: deposit failed to unlock mutex\n");
    }
    
    pthread_exit(0);
}

/* Husband: withdraws from the shared account */
void *withdraw(void *param)
{
    printf("Start of withdraw().\n");
    
    int value = atoi(param);

    if (pthread_mutex_lock(&mutex) != 0) {
        printf("Error: withdraw failed to lock mutex\n");
        pthread_exit(0);
    }
 
    //acquire semaphore
    sem_wait(&sem);

    /* critical section */
    if (amount <= 0){ // only allow withdrawals if amount is larger than 0
        amount -= value;
        printf("Withdrawal amount = %d\n", amount);
    } else {
        printf("Couldn't withdraw; not enough funds.\n");
    }
    
    // release semaphore
    sem_post(&sem);

    if (pthread_mutex_unlock(&mutex) != 0) {
        printf("Error: withdraw failed to unlock mutex\n");
    }

    pthread_exit(0);
}
