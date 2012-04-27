#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

int *array;
int *search_results;

int search_key;
int random_int_range;
int partition_size;

unsigned long int array_size;
unsigned long int array_partitions;


void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void populate_array() {
    unsigned int i;
    for (i = 0; i < array_size; i++)
        array[i] = random() % random_int_range;
}

void search_array(void *current_partition) {
    int partition = (int) current_partition;
    unsigned long int i;
    unsigned long int starting_point = (partition_size *  partition) - 1;
    unsigned long int max_search_index = starting_point + partition_size;

    // check to see if the search key has been found already
    int last_result = -1;
    for (i = 0; last_result == -1 && i < partition-1; i++)
        last_result = search_results[i];

    // if the search key hasn't been found yet
    if (last_result == -1) {

        for (i = starting_point; i < max_search_index; i++) {
            if (array[i] == search_key) {
                //printf("FOUND at index %d, partition %d\n", i, partition);
                search_results[partition-1] = i;
                return;
            }
        }

        search_results[partition-1] = -1;
    }
}

int main(int argc, char *argv[]) {
    unsigned int i;
    pthread_t *threads;

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

    // no search results have been evaluated yet, so clear them all to -1
    for (i=0; i < array_partitions; i++)
        search_results[i] = -1;

    for (i=1; i <= array_partitions; i++)
        pthread_create(&threads[i-1], NULL, (void*) search_array, (void*) i);

    for (i=0; i < array_partitions; i++)
        pthread_join(threads[i], NULL);

    for (i=0; i < array_partitions; i++)
        printf("[%d] Result: %d\n", i, search_results[i]);

    free(array);
    free(search_results);
    free(threads);

    return 0;
}
