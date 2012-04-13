#include <stdio.h>
#include <stdlib.h>

#define REFERENCE_SIZE 100
#define MAX_PAGE_NUMBER 13
#define MAX_PAGE_FRAMES 13

int page_reference[REFERENCE_SIZE];
int number_of_frames;
int *page_frames;

//----{ Helper Functions }------------------------------------------------------

void print_array(int *array, int size,  char *name) {
    int i;
    printf("----- \n%s:  ", name);
    for (i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n-----\n");
}

void populate_reference() {
    int i, value;
    //printf("POPULATING:  ");
    for (i = 0; i < REFERENCE_SIZE; i++) {
        value = (random() % MAX_PAGE_NUMBER) + 1;

        while (i > 0 && value == page_reference[i-1])
            value = (random() % MAX_PAGE_NUMBER) + 1;

        page_reference[i] = value;
        //printf("%d ", page_reference[i]);
    }
    //printf("\n");
}

void clear_page_frames() {
    int i;
    for (i = 0; i < number_of_frames; i++)
        page_frames[i] = -1;
}

int in_page_frames(int page_number) {
    int frame;
    for (frame = 0; frame < number_of_frames; frame++) {
        // Return frame number for LRU reference counting purposes
        if (page_frames[frame] == page_number)
            return frame+1;
    }

    //printf("%d is not in page_frames\n", page_number);
    return 0;
}

int get_furthest_reference(int starting_index, int page_number) {
    int i;
    for (i = starting_index; i < REFERENCE_SIZE; i++) {
        if (page_reference[i] == page_number)
            return i;
    }

    return REFERENCE_SIZE;
}

// pushes/moves frame_index to the top of the stack, shifts older numbers down as needed.
void push(int *stack, int frame_index) {

    int i, shifting_start = 0;
    for (i = 0; i < number_of_frames && shifting_start == 0; i++) {
        if (stack[i] == frame_index)
            shifting_start = i;
    }

    for (i = shifting_start+1; i < number_of_frames; i++)
        stack[i-1] = stack[i];

    stack[number_of_frames-1] = frame_index;

    //printf("LRU STACK:  ");
    //for (i=0; i < number_of_frames; i++)
    //    printf("%d ", stack[i]);
    //printf("\n");
}


//----{ Algorithms }------------------------------------------------------------
int FIFO() {
    int frame, reference_index, page_number, page_faults;
    int page_stored = 0;

    frame = 0;
    page_faults = 0;
    reference_index = 0;
    while (reference_index < REFERENCE_SIZE) {

        page_stored = 0;
        while (!page_stored && reference_index < REFERENCE_SIZE) {
            page_number = page_reference[reference_index];

            if (!in_page_frames(page_number)) {
                page_frames[frame] = page_number;
                page_faults += 1;
                page_stored = 1;
            }

            reference_index += 1;
        }

        if (frame+1 < number_of_frames)
            frame += 1;
        else
            frame = 0;
    }

    return page_faults;
}

int LRU() {
    int frame, reference_index, page_number, page_faults;
    int least_referenced[number_of_frames];
    int empty_frames = number_of_frames;
    int page_stored = 0;

    page_faults = 0;
    reference_index = 0;
    while (reference_index < REFERENCE_SIZE) {

        page_stored = 0;
        while (!page_stored && reference_index < REFERENCE_SIZE) {
            page_number = page_reference[reference_index];

            if (empty_frames > 0) {
                frame = number_of_frames - empty_frames;

                page_frames[frame] = page_number;
                least_referenced[frame] =  frame;

                page_faults += 1;
                empty_frames -= 1;
            }

            else {
                frame = in_page_frames(page_number);

                if (!frame) {
                    page_frames[ least_referenced[0] ] = page_number;
                    frame = least_referenced[0]+1;

                    page_faults += 1;
                    page_stored = 1;
                }

                push(least_referenced, frame-1);
            }

            reference_index += 1;
        }
    }

    return page_faults;
}

int OPT() {
    int frame, reference_index, page_number, page_faults;
    int furthest_indexed_frame, index, furthest_index;
    int empty_frames = number_of_frames;
    int page_stored = 0;

    page_faults = 0;
    reference_index = 0;
    while (reference_index < REFERENCE_SIZE) {

        page_stored = 0;
        while (!page_stored && reference_index < REFERENCE_SIZE) {
            page_number = page_reference[reference_index];

            if (empty_frames > 0) {
                frame = number_of_frames - empty_frames;

                page_frames[frame] = page_number;

                page_faults += 1;
                empty_frames -= 1;
            }

            else {
                frame = in_page_frames(page_number);

                if (!frame) {
                    page_frames[furthest_indexed_frame] = page_number;
                    page_faults += 1;
                    page_stored = 1;
                }
            }

            reference_index += 1;

            // Update the furthest index
            furthest_index = -1;
            for (frame = 0; frame < (number_of_frames - empty_frames); frame++) {
                index = get_furthest_reference(reference_index, page_frames[frame]);

                if (index > furthest_index) {
                    furthest_index = index;
                    furthest_indexed_frame = frame;
                }
            }
            //print_array(page_reference, REFERENCE_SIZE, "REFERENCE");
            //printf("Page Number = %d\n", page_number);
            //printf("FURTHEST REFERENCE INDEX: %d\n", furthest_index);
            //
            //print_array(page_frames, number_of_frames, "FRAME");
            //printf("Furthest indexed frame: %d\n\>>>>\n", furthest_indexed_frame);
        }
    }

    return page_faults;
}

//----{ Main }------------------------------------------------------------------
int main() {
    int fifo, lru, opt;
    FILE *results;

    populate_reference();

    page_frames = malloc(sizeof(int));

    results = fopen("output/virtual_memory.csv", "w");
    fprintf(results, "Frames,FIFO,LRU,OPT\n");

    for (number_of_frames = 1; number_of_frames <= MAX_PAGE_FRAMES; number_of_frames++) {
        page_frames = realloc(page_frames, number_of_frames * sizeof(int));
        clear_page_frames();

        fifo = FIFO();
        lru = LRU();
        opt = OPT();

        fprintf(results, "%d,%d,%d,%d\n", number_of_frames, fifo, lru, opt);

        printf("=== Using %d Page Frames ===\n", number_of_frames);
        printf("FIFO: %d page faults\n", fifo);
        printf("LRU:  %d page faults\n", lru);
        printf("OPT:  %d page faults\n\n", opt);
    }

    fclose(results);
    free(page_frames);

    return 0;
}
