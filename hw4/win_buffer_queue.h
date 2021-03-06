#include <windows.h>

typedef int Buffer;

typedef struct {

    Buffer *queue;
    int size;
    int count;

    int in;
    int out;

    // Counting semaphores
    HANDLE has_empty_buffers;
    HANDLE has_full_buffers;

    // Mutex lock
    HANDLE lock;
        
} BufferQueue;
