#include <stdio.h>
#include <windows.h>

HANDLE mutex_lock;

int buffer_size, producer_count, consumer_count;
int to_produce, to_consume, p_wait, c_wait;

int buffer_count = 0;
int *buffer_queue;

void check(int expression, char *message) {
    if (!expression) {
        DWORD error = GetLastError();
        fprintf(stderr, "%s: %x\n", message, error);
        exit(error);
    }
}

// Adds a buffer to the queue
void push() {
    if (buffer_count < buffer_size) {
        buffer_count += 1;
        buffer_queue[buffer_count] = 1;
    }
}

// Removes a buffer from the queue
void pop() {
    if (buffer_count > 0) {
        buffer_count -= 1;
        buffer_queue[buffer_count] = 0;
    }
}

void producer() {
    int i;
    
    fprintf(stderr, "**Producer started**\n");
    do {
        WaitForSingleObject(mutex_lock, INFINITE);

        for (i = 0; i < to_produce; i++)
            push();
        
        fprintf(stderr, "[PRODUCER] Buffer count increased \t Count: %d\n", buffer_count);

        ReleaseMutex(mutex_lock);
        Sleep(p_wait);

    } while (buffer_count < buffer_size);
}

void consumer() {
    int i;
    
    fprintf(stderr, "**Consumer started**\n");
    do {
        WaitForSingleObject(mutex_lock);

        for (i = 0; i < to_consume; i++)
            pop();
        
        fprintf(stderr, "[CONSUMER] Buffer count decreased \t Count: %d\n", buffer_count);

        ReleaseMutex(mutex_lock);
        Sleep(c_wait);

    } while (buffer_count > 0);
}


int main(int argc, char *argv[]) {

    int i;
    time_t start_time, end_time;
    HANDLE *producers, *consumers;

    check(argc == 7,
        "Requires the arguments: \nBuffer Size, # of Producers, # of Consumers, # of Products, P-wait, C-wait\n");

    buffer_size  = atoi(argv[1]);
    
    producer_count = atoi(argv[2]);
    consumer_count = atoi(argv[3]);

    to_produce = atoi(argv[4]);
    to_consume = producer_count * (to_produce / consumer_count);

    p_wait = atoi(argv[5]);
    c_wait = atoi(argv[6]);

    buffer_queue = malloc(sizeof(int) * buffer_size);
    
    producers = malloc(sizeof(HANDLE) * producer_count);
    consumers = malloc(sizeof(HANDLE) * consumer_count);

    mutex_lock = CreateMutex(NULL, FALSE, NULL);

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

    free(buffer_queue);
    free(producers);
    free(consumers);
    CloseHandle(mutex_lock);
    return 0;
}
