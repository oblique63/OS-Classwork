#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_N 3

int *array;
int array_size;
int array_partitions;
int partition_size;

int search_key;
int *search_results;

int random_int_range;


void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void populate_array() {
    int i;
    for (i = 0; i < array_size; i++)
        array[i] = random() % random_int_range;

    // clear search results
    for (i = 0; i < array_partitions; i++)
        search_results[i] = -1;
}

void search_array(void *current_partition) {
    int partition = (int) current_partition;
    int i;
    int starting_point = (partition_size *  partition) - 1;
    int max_search_index = starting_point + partition_size;

    for (i = starting_point; i < max_search_index; i++) {
        if (array[i] == search_key) {
            search_results[partition-1] = i;
            return;
        }
    }

    search_results[partition-1] = -1;
}

int main(int argc, char *argv[]) {
    int i, P, N;
    int error_count;
    int partition_with_key;
    pthread_t *threads;
    struct timeval start_time, end_time;
    long int total_time;
    FILE *results;


    if (strcmp(argv[1], "single") == 0)
        results = fopen("output/unix_search_singlecore.csv", "w");
    else
        results = fopen("output/unix_search_multicore.csv", "w");

    fprintf(results, "Threads,Time\n");

    P = 18;
    array_size = pow(2, P);
    array = malloc(sizeof(int) * array_size);

    search_results = malloc(sizeof(int));
    threads = malloc(sizeof(pthread_t));

    for (N = 0; N <= MAX_N; N++) {
        array_partitions = pow(2, N);
        partition_size = array_size / array_partitions;

        search_results = realloc(search_results, sizeof(int) * array_partitions);
        threads = realloc(threads, sizeof(pthread_t) * array_partitions);

        // limit random-ness of the integers to a reasonable domain
        // that may actually be found once in a while
        random_int_range = array_size * 2;

        srandom ( clock() * time(NULL) );
        populate_array();
        search_key = random() % random_int_range;

        printf("\n$ ./unix_search %d %d\n", P, N);

        printf("SEARCH KEY: %d\n", search_key);

        gettimeofday(&start_time, NULL);

        for (i=1; i <= array_partitions; i++)
            pthread_create(&threads[i-1], NULL, (void*) search_array, (void*) i);

        for (i=0; i < array_partitions; i++)
            pthread_join(threads[i], NULL);

        gettimeofday(&end_time, NULL);
        total_time = end_time.tv_usec - start_time.tv_usec;

        error_count = 0;
        partition_with_key = -1;
        for (i=0; i < array_partitions; i++) {
            printf("[Partition %d] Result: %d\n", i+1, search_results[i]);

            if (search_results[i] != -1) {
                if (partition_with_key == -1)
                    partition_with_key = i+1;
                else
                    error_count += 1;
            }
        }

        if (partition_with_key != -1) {
            printf("\nSearch Key found in partition %d at index %d\n", partition_with_key, search_results[partition_with_key-1]);
            printf("Index contents: %d\n", array[ search_results[partition_with_key-1] ]);
            printf("Errors: %d\n", error_count);
        }

        printf("\nTime: %ld micro-seconds\n", total_time);

        fprintf(results, "%d,%ld\n", array_partitions, total_time);
    }

    fclose(results);
    free(array);
    free(search_results);
    free(threads);

    return 0;
}
