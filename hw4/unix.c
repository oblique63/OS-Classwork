#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

pthread_mutex_t lock;

int buffer_size, producer_count, consumer_count;
int to_produce, to_consume, p_wait, c_wait;

int buffer_count = 0;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void producer() {
    fprintf(stderr, "Producer started");

    do {
        pthread_mutex_lock(&lock);

        buffer_count += to_produce;
        fprintf(stderr, "[PRODUCER] Buffer count increased");

        pthread_mutex_unlock(&lock);
        sleep(p_wait);

    } while (buffer_count < buffer_size);
}

void consumer() {
    fprintf(stderr, "Consumer started");

    do {
        pthread_mutex_lock(&lock);

        buffer_count -= to_consume;
        fprintf(stderr, "[CONSUMER] Buffer count decreased");
        
        pthread_mutex_unlock(&lock);
        sleep(c_wait);

    } while (buffer_count > 0);
}


int main(int argc, char *argv[]) {

    int i;
    time_t start_time, end_time;
    pthread_t *producers, *consumers;

    check(argc == 7,
        "Requires the arguments:\n Buffer Size, # of Producers, # of Consumers, # of Products, P-wait, C-wait");

    buffer_size  = atoi(argv[1]);
    producer_count = atoi(argv[2]);
    consumer_count = atoi(argv[3]);

    to_produce = atoi(argv[4]);
    to_consume = producer_count * (to_produce / consumer_count);

    p_wait = atoi(argv[5]);
    c_wait = atoi(argv[6]);

    producers = malloc(sizeof(pthread_t * producer_count));
    consumers = malloc(sizeof(pthread_t * consumer_count));

    pthread_mutex_init(&lock, NULL);

    start_time = time(NULL);
    printf("Start time: %s\n", ctime(start_time));
    
    for (i = 0; i < producer_count; i++)
        pthread_create(&producers[i], NULL, (void *) producer, NULL);

    for (i = 0; i < consumer_count; i++)
        pthread_create(&consumers[i], NULL, (void *) consumer, NULL);


    for (i = 0; i < producer_count; i++)
        pthread_join(producers[i], NULL);

    for (i = 0; i < consumer_count; i++)
        pthread_join(consumers[i], NULL);

    end_time = time(NULL);
    printf("End time: %s\n", ctime(end_time));
    printf("Duration: %d seconds\n", end_time - start_time)

    pthread_mutex_destroy(&lock);
    return 0;
}
