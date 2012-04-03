#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "unix_buffer_queue.h"

BufferQueue buffer_queue;

int producer_count, consumer_count;
int to_produce, to_consume, p_wait, c_wait;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void producer(void * producer_id) {
    int i;
    int id = (int) producer_id;

    fprintf(stderr, "**Producer #%d started**\n", id);
    do {
        sem_wait(&buffer_queue.has_new_empty_buffers);
        pthread_mutex_lock(&buffer_queue.lock);

        for (i = 0; i < to_produce; i++)
            push(buffer_queue, i);  // add a value to the top of the queue

        fprintf(stderr, "[PRODUCER #%d] Buffer count increased \t Count: %d\n", id, buffer_queue.count);

        pthread_mutex_unlock(&buffer_queue.lock);
        sem_post(&buffer_queue.has_new_full_buffers);
        
        sleep(p_wait);

    } while (buffer_queue.count < buffer_queue.size);

    fprintf(stderr, "**Producer #%d exited**\n", id);
}

void consumer(void * consumer_id) {
    int i;
    int id = (int) consumer_id;

    fprintf(stderr, "**Consumer #%d started**\n", id);
    do {
        sem_wait(&buffer_queue.has_new_full_buffers);
        pthread_mutex_lock(&buffer_queue.lock);

        for (i = 0; i < to_consume; i++)
            pop(buffer_queue);  // remove a value from the top of the queue

        fprintf(stderr, "[CONSUMER #%d] Buffer count decreased \t Count: %d\n", id, buffer_queue.count);

        pthread_mutex_unlock(&buffer_queue.lock);
        sem_post(&buffer_queue.has_new_empty_buffers);
        
        sleep(c_wait);

    } while (buffer_queue.count > 0);

    fprintf(stderr, "**Consumer #%d exited**\n", id);
}

int main(int argc, char *argv[]) {

    time_t start_time, end_time;

    check(argc == 7,
        "Requires the arguments: \nBuffer Size, # of Producers, # of Consumers, # of Products, P-wait, C-wait\n");

    buffer_queue.size  = atoi(argv[1]);
    producer_count = atoi(argv[2]);
    consumer_count = atoi(argv[3]);

    to_produce = atoi(argv[4]);
    to_consume = producer_count * (to_produce / consumer_count);

    p_wait = atoi(argv[5]);
    c_wait = atoi(argv[6]);

    buffer_queue.queue = malloc(sizeof(Buffer) * buffer_queue.size);

    pthread_t producers[producer_count];
    pthread_t consumers[consumer_count];

    // Mutex Initialization
    pthread_mutex_init(&buffer_queue.lock, NULL);

    // Semaphore Initialization
    sem_init(&buffer_queue.has_new_full_buffers, 0, 0);
    sem_init(&buffer_queue.has_new_empty_buffers, 0, buffer_queue.size);

    printf("Consumers to consume %d buffers\n", to_consume);
    
    start_time = time(NULL);
    printf("Start time: %s", ctime(&start_time));
    printf("====================\n");

    int i;
    for (i = 0; i < producer_count; i++)
        pthread_create(&producers[i], NULL, (void *) producer, (void *) i+1);

    for (i = 0; i < consumer_count; i++)
        pthread_create(&consumers[i], NULL, (void *) consumer, (void *) i+1);


    for (i = 0; i < producer_count; i++)
        pthread_join(producers[i], NULL);

    for (i = 0; i < consumer_count; i++)
        pthread_join(consumers[i], NULL);

    end_time = time(NULL);
    printf("====================\n");
    printf("End time: %s", ctime(&end_time));
    printf("Duration: %ld seconds\n", end_time - start_time);

    free(buffer_queue.queue);
    pthread_mutex_destroy(&buffer_queue.lock);

    return 0;
}