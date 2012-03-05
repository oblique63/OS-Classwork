#include <stdio.h>
#include <windows.h>

#define THREADS 10
int counter = 0;
int thread_num = 0;

void check(int expression, char *message, HANDLE thread) {
    if (!expression) {
        fprintf(stderr, "%s: %x\n", message, GetLastError());
        CloseHandle(thread);
        exit(return_code);
    }
}

void add_10() {
    DWORD thread_id = GetCurrentThreadId();
    int thread = ++thread_num;
    int local_counter = counter;
    
    printf("[Thread #%d] \t ID: %ld \t Counter Before: %d\n", thread, thread_id, local_counter);
    local_counter += 10;
    printf("[Thread #%d] \t ID: %ld \t Counter After: %d\n", thread, thread_id, local_counter);
    // Ensure race conditions by waiting to update the counter
    Sleep(1);
    counter = local_counter;
}

int main(int argc, char *argv[]) {

    int i;
    HANDLE threads[THREADS];

    for (i=0; i < THREADS; i++) {
        threads[i] = CreateThread(NULL, 0, add_10, NULL, 0, NULL);
        
        check(threads[i] != NULL, "Failed to create thread.", threads[i]);
    }

    for (i=0; i < THREADS; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }

    return 0;
}
