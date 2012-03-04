#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 10
int counter = 0;
int thread_num = 0;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void add_10() {
    unsigned long thread_id = -(unsigned long) pthread_self();
    
    thread_num += 1;
    int thread = thread_num;
    int local_counter = counter;

    printf("[Thread #%d] \t ID: %ld \t Counter Before: %d\n", thread, thread_id, local_counter);
    local_counter += 10;
    printf("[Thread #%d] \t ID: %ld \t Counter After: %d\n", thread, thread_id, local_counter);

    counter = local_counter;
}

int main() {
    int i, return_code;
    pthread_t threads[THREADS];

    for (i=0; i < THREADS; i++) {
        return_code = pthread_create(&threads[i], NULL, (void*) add_10, NULL);
        check(return_code == 0, "Thread Failed");
    }

    // wait for any remaining threads still running
    for (i=0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("\tCounter Total: %d\n", counter);
    return 0;
}
