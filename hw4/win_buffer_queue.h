#include <windows.h>

typedef int Buffer;

typedef struct {

    Buffer *queue;
    int size;
    int count;

    // Counting semaphores
    HANDLE has_new_empty_buffers;
    HANDLE has_new_full_buffers;

    // Mutex lock
    HANDLE lock;
        
} BufferQueue;


// Adds a buffer to the queue
void push(BufferQueue *buffer_queue, Buffer buffer) {
    if (buffer_queue->count < buffer_queue->size) {
        buffer_queue->count += 1;
        buffer_queue->queue[buffer_queue->count] = buffer;
    }
}

// Removes a buffer from the queue
void pop(BufferQueue *buffer_queue) {
    if (buffer_queue->count > 0) {
        buffer_queue->count -= 1;
        buffer_queue->queue[buffer_queue->count] = (Buffer) 0;
    }
}
