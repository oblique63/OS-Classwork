#include <stdio.h>
#include <stdlib.h>

#define REFERENCE_SIZE 100
#define MAX_PAGE_NUMBER 13
#define MAX_PAGE_FRAMES 13

int page_reference[REFERENCE_SIZE];
int number_of_frames;
int *page_frames;


void populate_reference() {
    int i;
    for (i = 0; i < REFERENCE_SIZE; i++) {
        page_reference[i] = (random() % MAX_PAGE_NUMBER) + 1;
    }
}

int in_page_frame(int page_number) {
    int frame;
    for (frame = 0; frame < number_of_frames; frame++) {
        // Return frame number for LRU reference counting purposes
        if (page_frames[frame] == page_number)
            return frame+1;
    }

    return 0;
}


int FIFO() {
    int frame, reference_index, page_number, page_faults;
    int page_stored = 0;

    frame = 0;
    for (page_faults = 0; reference_index < REFERENCE_SIZE; page_faults++) {

        page_stored = 0;
        while (!page_stored && reference_index < REFERENCE_SIZE) {
            page_number = page_reference[reference_index];

            if (!in_page_frame(page_number, number_of_frames)) {
                page_frames[frame] = page_number;
                page_stored = 1;
            }

            reference_index += 1;
        }

        if (frame < number_of_frames)
            frame += 1;
        else
            frame = 0;
    }

    return page_faults;
}

int LRU() {
    int frame, reference_index, page_number, page_faults;
    int page_stored = 0;

    return page_faults;
}

int OPT() {
    int frame, reference_index, page_number, page_faults;
    int page_stored = 0;

    return page_faults;
}


int main() {
    populate_reference();

    for (number_of_frames = 1; number_of_frames <= MAX_PAGE_FRAMES; number_of_frames++) {

        page_frames = calloc(number_of_frames, sizeof(int));

        printf("\n=== Using %d Page Frames ===\n", number_of_frames);
        printf("FIFO: %d page faults\n", FIFO(number_of_frames));
        printf("LRU:  %d page faults\n", LRU(number_of_frames));
        printf("OPT:  %d page faults\n", OPT(number_of_frames);

        free(page_frames);
    }

    return 0;
}
