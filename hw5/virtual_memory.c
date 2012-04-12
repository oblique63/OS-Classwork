#include <stdio.h>
#include <stdlib.h>

#define REFERENCE_SIZE 100
#define MAX_PAGE_NUMBER 13
#define MAX_PAGE_FRAMES 13

int page_reference[REFERENCE_SIZE];

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void populate_reference() {
    int i;
    for (i = 0; i < REFERENCE_SIZE; i++) {
        page_reference[i] = (random() % MAX_PAGE_NUMBER) + 1;
    }
}

void FIFO() {}

void LRU() {}

void OPT() {}

int main() {
    populate_reference();

    return 0;
}
