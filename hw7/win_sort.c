#include <windows.h>
#include <math.h>

int *array;
int array_size;
int array_partitions;
int partition_size;
int *minimums;


void check(int expression, char *message) {
    if (!expression) {
        DWORD error = GetLastError();
        printf("%s: %x\n", message, error);
        exit(error);
    }
}

void populate_array() {
    int random_int_range = array_size * 2;
    int i;

    srand( time(NULL) );
    for (i = 0; i < array_size; i++)
        array[i] = rand();
}

void sort_partition(void *current_partition_index) {
    int partition = (int) current_partition_index;
    int starting_index = partition_size * partition;
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


int main(int argc, char *argv[]) {
    int i;
    int *sorted_array;
    HANDLE *threads;
    LARGE_INTEGER start_time, end_time, clock_frequency;
    double total_time;
    int total_time_rounded;

    check(argc == 3, "Needs P and N values");

    array_size = pow(2, atoi(argv[1]));
    array_partitions = pow(2, atoi(argv[2]));
    partition_size = array_size / array_partitions;

    array = malloc(sizeof(int) * array_size);
    sorted_array = malloc(sizeof(int) * array_size);
    minimums = malloc(sizeof(int) * array_partitions);
    threads = malloc(sizeof(HANDLE) * array_partitions);

    populate_array();

    QueryPerformanceFrequency(&clock_frequency);
    QueryPerformanceCounter(&start_time);

    for (i=0; i < array_partitions; i++)
        threads[i] = CreateThread(NULL, 0, (void *) sort_partition, (void*) i, 0, NULL);

    for (i=0; i < array_partitions; i++)
        WaitForSingleObject(threads[i], INFINITE);

    for (i=0; i < array_size; i++) {
        sorted_array[i] = get_next();

        if (i > 0)
            check(sorted_array[i-1] <= sorted_array[i], "There was an error sorting the array");
    }

    QueryPerformanceCounter(&end_time);

    total_time = (double) (end_time.QuadPart - start_time.QuadPart) / clock_frequency.QuadPart;

    // convert to micro-seconds
    total_time *= 1000000.0;
    total_time_rounded = total_time + 0.5;

    puts("Sort was successful.");
    printf("Time: %ld micro-seconds\n", total_time_rounded);

    free(array);
    free(sorted_array);
    free(minimums);
    free(threads);

    return 0;
}
