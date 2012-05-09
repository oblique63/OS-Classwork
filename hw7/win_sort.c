#include <windows.h>
#include <math.h>

int *array;
int array_size;
int array_partitions;
int partition_size;

int search_key;
int *search_results;

int random_int_range;


void check(int expression, char *message) {
    if (!expression) {
        DWORD error = GetLastError();
        printf("%s: %x\n", message, error);
        exit(error);
    }
}

int random() {
    return (rand() * rand()) % random_int_range;
}

void populate_array() {
    int i;
    for (i = 0; i < array_size; i++)
        array[i] = random();
}

void search_array(void *current_partition) {
    int partition = (int) current_partition;
    int i;
    int starting_point = partition_size *  (partition-1);
    int max_search_index = (starting_point + partition_size) - 1;

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
    HANDLE *threads;
    LARGE_INTEGER start_time, end_time, clock_frequency;
    double total_time;
    int total_time_rounded;

    check(argc == 3, "Needs P and N values");

    array_size = pow(2, atoi(argv[1]));
    array_partitions = pow(2, atoi(argv[2]));
    partition_size = array_size / array_partitions;

    array = malloc(sizeof(int) * array_size);
    search_results = malloc(sizeof(int) * array_partitions);
    threads = malloc(sizeof(HANDLE) * array_partitions);

    // limit random-ness of the integers to a reasonable domain
    // that may actually be found once in a while
    random_int_range = array_size * 2;

    srand((unsigned) time(NULL));
    populate_array();
    search_key = random();

    printf("SEARCH KEY: %ld\n", search_key);

    QueryPerformanceFrequency(&clock_frequency);
    
    QueryPerformanceCounter(&start_time);

    for (i=1; i <= array_partitions; i++)
        threads[i-1] = CreateThread(NULL, 0, (void *) search_array, (void*) i, 0, NULL);

    for (i=0; i < array_partitions; i++)
        WaitForSingleObject(threads[i], INFINITE);
    
    QueryPerformanceCounter(&end_time);

    total_time = (double) (end_time.QuadPart - start_time.QuadPart) / clock_frequency.QuadPart;

    // convert to micro-seconds
    total_time *= 1000000.0;
    total_time_rounded = total_time + 0.5;

    for (i=0; i < array_partitions; i++) {
        printf("[Partition %ld] Result: %ld\n", i+1, search_results[i]);

        if (search_results[i] != -1) {
            if (partition_with_key == -1)
                partition_with_key = i+1;
            else
                error_count += 1;
        }
    }

    if (partition_with_key != -1) {
        printf("\nSearch Key found in partition %ld at index %ld\n", partition_with_key, search_results[partition_with_key-1]);
        printf("Index contents: %ld\n", array[ search_results[partition_with_key-1] ]);
        printf("Errors: %ld\n", error_count);
    }

    printf("\nTime: %ld micro-seconds\n", total_time_rounded);

    free(array);
    free(search_results);
    free(threads);

    return 0;
}
