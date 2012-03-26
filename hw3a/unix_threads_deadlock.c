#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 10
int counter = 0;
int thread_num = 0;
pthread_mutex_t lock;


void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void add_10() {
    // Get the absolute value of the thread id, because it might come back
    // negative on some systems.
    unsigned long thread_id = abs((unsigned long) pthread_self());

    pthread_mutex_lock(&lock);
    
    thread_num += 1;
    int thread = thread_num;
    int local_counter = counter;

    printf("[Thread #%d] \t ID: %ld \t Counter Before: %d\n", thread, thread_id, local_counter);
    local_counter += 10;
    printf("[Thread #%d] \t ID: %ld \t Counter After: %d\n", thread, thread_id, local_counter);
    // Provoke race conditions by waiting to update the counter
    sleep(1);
    counter = local_counter;

    // By not unlocking the mutex, other threads will be stuck, perpetually
    // waiting for their turn to access the shared data.
    //pthread_mutex_unlock(&lock);
}

int main() {
    int i, return_code;
    pthread_t threads[THREADS];

    pthread_mutex_init(&lock, NULL);
    
    for (i=0; i < THREADS; i++) {
        return_code = pthread_create(&threads[i], NULL, (void*) add_10, NULL);
        check(return_code == 0, "Thread Failed");
    }

    // wait for any remaining threads still running
    for (i=0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    
    printf("\tCounter Total: %d\n", counter);
    return 0;
}
