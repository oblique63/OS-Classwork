#include <stdio.h>
#include <stdlib.h>
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
    counter += 10;
    thread_num += 1;
    printf("Hello, I'm thread #%d.  ID: %ld   Counter Value: %d\n", thread_num, (unsigned long int) pthread_self(), counter);
}

int main() {
    int i, return_code;
    pthread_t threads[THREADS];
    
    for (i=0; i < THREADS; i++) {
        return_code = pthread_create(&threads[i], NULL, (void*) add_10, NULL);
        check(return_code == 0, "Thread Failed");
        pthread_join(threads[i], NULL);
    }

    printf("Counter Total: %d\n", counter);
    return 0;
}
