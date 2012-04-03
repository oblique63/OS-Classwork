#include <stdio.h>
#include <windows.h>
#include "win_buffer_queue.h"

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

        // Wait() / P()
        WaitForSingleObject(buffer_queue.has_new_empty_buffers, 0);
        WaitForSingleObject(buffer_queue.lock, INFINITE);

        for (i = 0; i < to_produce; i++)
            push(&buffer_queue, i);  // add a value to the top of the queue

        fprintf(stderr, "[PRODUCER #%d] Buffer count increased \t Count: %d\n", id, buffer_queue.count);

        // Signal() / V()
        ReleaseMutex(buffer_queue.lock);
        ReleaseSemaphore(buffer_queue.has_new_full_buffers, 1, NULL);
        
        Sleep(p_wait);

    } while (buffer_queue.count < buffer_queue.size);

    fprintf(stderr, "--Producer #%d exited--", id);
}

void consumer(void * consumer_id) {
    int i;
    int id = (int) consumer_id;

    fprintf(stderr, "**Consumer #%d started**\n", id);
    do {

        // Wait() / P()
        WaitForSingleObject(buffer_queue.has_new_full_buffers, 0);
        WaitForSingleObject(buffer_queue.lock, INFINITE);

        for (i = 0; i < to_consume; i++)
            pop(&buffer_queue);  // remove a value from the top of the queue

        fprintf(stderr, "[CONSUMER #%d] Buffer count decreased \t Count: %d\n", id, buffer_queue.count);

        // Signal() / V()
        ReleaseMutex(buffer_queue.lock);
        ReleaseSemaphore(buffer_queue.has_new_empty_buffers);
        
        Sleep(c_wait);

    } while (buffer_queue.count > 0);

    fprintf(stderr, "--Consumer #%d exited--\n", id);
}

int main(int argc, char *argv[]) {

    int i;
    time_t start_time, end_time;
    HANDLE *producers, *consumers;

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

    producers = malloc(sizeof(HANDLE) * producer_count);
    consumers = malloc(sizeof(HANDLE) * consumer_count);

    // Mutex Initialization
    buffer_queue.lock = CreateMutex(NULL, FALSE, NULL);

    // Semaphore Initialization
    buffer_queue.has_new_full_buffers =
            CreateSemaphore(NULL, 0, 0, NULL);
    buffer_queue.has_new_empty_buffers =
            CreateSemaphore(NULL, buffer_queue.size, buffer_queue.size, NULL);
    
    printf("Consumers to consume %d buffers\n", to_consume);
    
    start_time = time(NULL);
    printf("Start time: %s", ctime(&start_time));
    printf("====================\n");

    for (i = 0; i < producer_count; i++)
        producers[i] = CreateThread(NULL, 0, (void *) producer, NULL, 0, NULL);

    for (i = 0; i < consumer_count; i++)
        consumers[i] = CreateThread(NULL, 0, (void *) consumer, NULL, 0, NULL);


    for (i = 0; i < producer_count; i++) {
        WaitForSingleObject(producers[i], INFINITE);
        CloseHandle(producers[i]);
    }

    for (i = 0; i < consumer_count; i++) {
        WaitForSingleObject(consumers[i], INFINITE);
        CloseHandle(consumers[i]);
    }

    end_time = time(NULL);
    printf("====================\n");
    printf("End time: %s", ctime(&end_time));
    printf("Duration: %ld seconds\n", end_time - start_time);

    free(producers);
    free(consumers);
    free(buffer_queue.queue);
    CloseHandle(buffer_queue.lock);
    CloseHandle(buffer_queue.has_new_full_buffers);
    CloseHandle(buffer_queue.has_new_empty_buffers);
    
    return 0;
}
