#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

// Adds 'items' # of buffers to the queue
void push(int items, int *buffer_count) {
    int i;

    for (i = 0; i < items && buffer_queue.count < buffer_queue.size; i++) {
        // sleep to provoke race conditions
        sleep(1);

        buffer_queue.queue[buffer_queue.in] = (Buffer) i;
        buffer_queue.in = (buffer_queue.in+1) % buffer_queue.size;
        buffer_queue.count += 1;
    }
    
    *buffer_count = buffer_queue.count;
}

// Removes 'items' # of buffers from the queue
void pop(int items, int *buffer_count) {
    int i;

    for (i = 0; i < items && buffer_queue.count > 0; i++) {
        buffer_queue.queue[buffer_queue.out] = (Buffer) 0;
        buffer_queue.out = (buffer_queue.out+1) % buffer_queue.size;
        buffer_queue.count -= 1;
        
        // sleep to provoke race conditions
        sleep(1);
    }

    *buffer_count = buffer_queue.count;
}

void producer(void * producer_id) {
    int id = (int) producer_id;
    int buffer_count;

    fprintf(stderr, "**Producer #%d started**\n", id);
    do {
        // Busy wait
        sleep(p_wait);
        
        // add a value to the top of the queue
        push(to_produce, &buffer_count);

        fprintf(stderr, "[PRODUCER #%d] Buffers added to queue \t\t Count: %d\n", id, buffer_queue.count);

    } while (buffer_count < buffer_queue.size);

    fprintf(stderr, "--Producer #%d exited--\n", id);
}

void consumer(void * consumer_id) {
    int id = (int) consumer_id;
    int buffer_count;

    fprintf(stderr, "**Consumer #%d started**\n", id);
    do {
        // Busy wait
        sleep(c_wait);
        
        // remove a value from the top of the queue
        pop(to_consume, &buffer_count);

        fprintf(stderr, "[CONSUMER #%d] Buffers removed from queue \t Count: %d\n", id, buffer_queue.count);

    } while (buffer_count > 0);

    fprintf(stderr, "--Consumer #%d exited--\n", id);
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

    return 0;
}
