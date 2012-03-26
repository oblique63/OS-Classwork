#include <stdio.h>
#include <windows.h>

#define THREADS 10
int counter = 0;
int thread_num = 0;

HANDLE mutex_lock;

void check(int expression, char *message, HANDLE thread) {
    if (!expression) {
        DWORD error = GetLastError();
        fprintf(stderr, "%s: %x\n", message, error);
        CloseHandle(thread);
        CloseHandle(mutex_lock);
        exit(error);
    }
}

void add_10() {
    DWORD thread_id;
    int thread, local_counter;
    
    WaitForSingleObject(mutex_lock, INFINITE);

    thread_id = GetCurrentThreadId();
    thread = ++thread_num;
    local_counter = counter;

    printf("[Thread #%d] \t ID: %ld \t Counter Before: %d\n", thread, thread_id, local_counter);
    local_counter += 10;
    printf("[Thread #%d] \t ID: %ld \t Counter After: %d\n", thread, thread_id, local_counter);
    // Provoke race conditions by waiting to update the counter.
    // This should delay it enough so that other threads end up assigning the old counter value
    // to their local variables.
    Sleep(1);
    counter = local_counter;

    ReleaseMutex(mutex_lock);
}

int main(int argc, char *argv[]) {

    int i;
    HANDLE threads[THREADS];

    mutex_lock = CreateMutex(NULL, FALSE, NULL);

    // By not unlocking this mutex, the child threads will be stuck, perpetually
    // waiting for their turn to access the shared data.
    WaitForSingleObject(mutex_lock, INFINITE);
    
    for (i=0; i < THREADS; i++) {
        threads[i] = CreateThread(NULL, 0, (void *) add_10, NULL, 0, NULL);

        check(threads[i] != NULL, "Failed to create thread.", threads[i]);
    }

    for (i=0; i < THREADS; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }

    CloseHandle(mutex_lock);

    printf("\tCounter Total: %d\n", counter);
    return 0;
}
