#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

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
    int i;
    int error_count = 0;
    int partition_with_key = -1;
    pthread_t *threads;
    clock_t start_time, end_time;
    double total_time;

    check(argc == 3, "Needs P and N values");

    array_size = pow(2, atoi(argv[1]));
    array_partitions = pow(2, atoi(argv[2]));
    partition_size = array_size / array_partitions;

    array = malloc(sizeof(int) * array_size);
    search_results = malloc(sizeof(int) * array_partitions);
    threads = malloc(sizeof(pthread_t) * array_partitions);

    // limit random-ness of the integers to a reasonable domain
    // that may actually be found once in a while
    random_int_range = array_size * 2;

    srandom ( time(NULL) );
    populate_array();
    search_key = random() % random_int_range;

    printf("SEARCH KEY: %d\n", search_key);

    start_time = clock();

    for (i=1; i <= array_partitions; i++)
        pthread_create(&threads[i-1], NULL, (void*) search_array, (void*) i);

    for (i=0; i < array_partitions; i++)
        pthread_join(threads[i], NULL);

    end_time = clock();
    total_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

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

    printf("\nTime: %f seconds\n", total_time);

    free(array);
    free(search_results);
    free(threads);

    return 0;
}
