#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;

int buffer_size, producer_count, consumer_count;
int to_produce, to_consume, p_wait, c_wait;

int buffer_count = 0;
int *buffer_queue;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
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
        pthread_mutex_lock(&lock);

        for (i = 0; i < to_produce; i++)
            push();

        fprintf(stderr, "[PRODUCER] Buffer count increased \t Count: %d\n", buffer_count);

        pthread_mutex_unlock(&lock);
        sleep(p_wait);

    } while (buffer_count < buffer_size);

    fprintf(stderr, "**Producer exited**\n");
}

void consumer() {
    int i;

    fprintf(stderr, "**Consumer started**\n");
    do {
        pthread_mutex_lock(&lock);

        for (i = 0; i < to_consume; i++)
            pop();

        fprintf(stderr, "[CONSUMER] Buffer count decreased \t Count: %d\n", buffer_count);

        pthread_mutex_unlock(&lock);
        sleep(c_wait);

    } while (buffer_count > 0);

    fprintf(stderr, "**Consumer exited**\n");
}

int main(int argc, char *argv[]) {

    time_t start_time, end_time;

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

    pthread_t producers[producer_count];
    pthread_t consumers[consumer_count];

    pthread_mutex_init(&lock, NULL);

    printf("Consumers to consume %d buffers\n", to_consume);
    
    start_time = time(NULL);
    printf("Start time: %s", ctime(&start_time));
    printf("====================\n");

    int i;
    for (i = 0; i < producer_count; i++)
        pthread_create(&producers[i], NULL, (void *) producer, NULL);

    for (i = 0; i < consumer_count; i++)
        pthread_create(&consumers[i], NULL, (void *) consumer, NULL);


    for (i = 0; i < producer_count; i++) {
        pthread_join(producers[i], NULL);
    }

    for (i = 0; i < consumer_count; i++) {
        pthread_join(consumers[i], NULL);
    }

    end_time = time(NULL);
    printf("====================\n");
    printf("End time: %s", ctime(&end_time));
    printf("Duration: %ld seconds\n", end_time - start_time);

    free(buffer_queue);
    pthread_mutex_destroy(&lock);
    return 0;
}
