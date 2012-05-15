#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int *array;
int array_size;
int array_partitions;
int partition_size;

int *minimums;


void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void populate_array() {
    int random_int_range = array_size * 2;
    int i;

    srandom ( time(NULL) );
    for (i = 0; i < array_size; i++)
        array[i] = random() % random_int_range;
}

// Insertion Sort
void sort_partition(void *current_partition_index) {
    int partition = (int) current_partition_index;
    int starting_index = partition_size *  partition;
    int ending_index = (starting_index + partition_size) - 1;
    int i, j, smaller_value;

    for (i = starting_index+1; i <= ending_index; i++) {
        if (array[i] < array[i-1]) {
            smaller_value = array[i];

            for (j = i; array[j-1] > smaller_value && j > starting_index; j--) {
                array[j] = array[j-1];
            }

            array[j] = smaller_value;
        }
    }

    minimums[partition] = starting_index;
}

int get_next() {
    int min_value = minimums[0];
    int min_index = 0;
    int value = 0;
    int i, max_partition_index;

    for (i = 1; i < array_partitions; i++) {
        value = array[ minimums[i] ];
        max_partition_index = ((i+1) * partition_size) - 1;

        if (value < min_value && minimums[i] < max_partition_index) {
            min_value = value;
            min_index = i;
        }
    }

    minimums[min_index] += 1;
    return min_value;
}

// Returns elapsed time in micro-seconds
int time_difference (struct timeval start_time, struct timeval end_time) {
    if (end_time.tv_usec < start_time.tv_usec) {
        int nsec = (start_time.tv_usec - end_time.tv_usec) / 1000000 + 1;
        start_time.tv_usec -= 1000000 * nsec;
        start_time.tv_sec += nsec;
    }

    if (end_time.tv_usec - start_time.tv_usec > 1000000) {
        int nsec = (end_time.tv_usec - start_time.tv_usec) / 1000000;
        start_time.tv_usec += 1000000 * nsec;
        start_time.tv_sec -= nsec;
    }

    return end_time.tv_usec - start_time.tv_usec;
}


int main(int argc, char *argv[]) {
    int i;
    int *sorted_array;
    pthread_t *threads;
    struct timeval start_time, end_time;
    unsigned long int total_time;

    check(argc == 3, "Needs P and N values");

    array_size = pow(2, atoi(argv[1]));
    array_partitions = pow(2, atoi(argv[2]));
    partition_size = array_size / array_partitions;

    array = malloc(sizeof(int) * array_size);
    sorted_array = malloc(sizeof(int) * array_size);
    minimums = malloc(sizeof(int) * array_partitions);
    threads = malloc(sizeof(pthread_t) * array_partitions);

    populate_array();

    gettimeofday(&start_time, NULL);

    for (i=0; i < array_partitions; i++)
        pthread_create(&threads[i], NULL, (void*) sort_partition, (void*) i);

    for (i=0; i < array_partitions; i++)
        pthread_join(threads[i], NULL);

    for (i=0; i < array_size; i++) {
        sorted_array[i] = get_next();

        if (i > 0)
            check(sorted_array[i] >= sorted_array[i-1], "There was an error sorting the array");
    }

    gettimeofday(&end_time, NULL);
    total_time = time_difference(start_time, end_time);

    puts("Sort was successful.");
    printf("Time: %ld micro-seconds\n", total_time);

    free(array);
    free(sorted_array);
    free(minimums);
    free(threads);

    return 0;
}
