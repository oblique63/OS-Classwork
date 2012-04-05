#include <stdio.h>
#include <windows.h>
#include "win_buffer_queue.h"

BufferQueue buffer_queue;

int producer_count, consumer_count;
int to_produce, to_consume, p_wait, c_wait;

void check(int expression, char *message) {
    if (!expression) {
        DWORD error = GetLastError();
        fprintf(stderr, "%s: %x\n", message, error);
        CloseHandle(buffer_queue.lock);
        CloseHandle(buffer_queue.has_full_buffers);
        CloseHandle(buffer_queue.has_empty_buffers);
        exit(error);
    }
}


// Adds 'items' # of buffers to the queue
void push(int items, int *buffer_count) {
    int i;

    WaitForSingleObject(buffer_queue.has_empty_buffers, 0);
    WaitForSingleObject(buffer_queue.lock, INFINITE);

    for (i = 0; i < items && buffer_queue.count < buffer_queue.size; i++) {
        // sleep to provoke race conditions
        Sleep(1);

        buffer_queue.queue[buffer_queue.in] = (Buffer) i;
        buffer_queue.in = (buffer_queue.in+1) % buffer_queue.size;
        buffer_queue.count += 1;
    }

    *buffer_count = buffer_queue.count;
    ReleaseMutex(buffer_queue.lock);
    ReleaseSemaphore(buffer_queue.has_full_buffers);
}

// Removes 'items' # of buffers from the queue
void pop(int items, int *buffer_count) {
    int i;

    WaitForSingleObject(buffer_queue.has_full_buffers, 0);
    WaitForSingleObject(buffer_queue.lock, INFINITE);

    for (i = 0; i < items && buffer_queue.count > 0; i++) {
        buffer_queue.queue[buffer_queue.out] = (Buffer) 0;
        buffer_queue.out = (buffer_queue.out+1) % buffer_queue.size;
        buffer_queue.count -= 1;

        // sleep to provoke race conditions
        Sleep(1);
    }

    *buffer_count = buffer_queue.count;
    ReleaseMutex(buffer_queue.lock);
    ReleaseSemaphore(buffer_queue.has_empty_buffers);
}

void producer(void * producer_id) {
    int id = (int) producer_id;
    int buffer_count;

    fprintf(stderr, "**Producer #%d started**\n", id);
    do {
        // Busy wait
        Sleep(p_wait);

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
        Sleep(c_wait);

        pop(to_consume, &buffer_count);

        fprintf(stderr, "[CONSUMER #%d] Buffers removed from queue \t Count: %d\n", id, buffer_queue.count);

    } while (buffer_count > 0);

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
    to_consume = producer_count * (to_produce / consumer_count);  // X * P/C

    p_wait = atoi(argv[5]);
    c_wait = atoi(argv[6]);

    buffer_queue.queue = malloc(sizeof(Buffer) * buffer_queue.size);

    producers = malloc(sizeof(HANDLE) * producer_count);
    consumers = malloc(sizeof(HANDLE) * consumer_count);

    // Mutex Initialization
    buffer_queue.lock = CreateMutex(NULL, FALSE, NULL);

    // Semaphore Initialization
    buffer_queue.has_full_buffers =
            CreateSemaphore(NULL, 0, 0, NULL);
    buffer_queue.has_empty_buffers =
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
    CloseHandle(buffer_queue.has_full_buffers);
    CloseHandle(buffer_queue.has_empty_buffers);

    return 0;
}
