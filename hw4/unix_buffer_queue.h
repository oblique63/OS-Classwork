#include <pthread.h>
#include <semaphore.h>

typedef int Buffer;

typedef struct {

    Buffer *queue;
    int size;
    int count;

    // Counting semaphores
    sem_t has_empty_buffers;
    sem_t has_full_buffers;

    // Mutex lock
    pthread_mutex_t lock;
    //sem_t lock;
        
} BufferQueue;
