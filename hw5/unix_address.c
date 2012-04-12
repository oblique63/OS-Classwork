#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 32000
#define PAGE_NUMBER_BITS 49
#define OFFSET_BITS 15

typedef unsigned long long int INT64;

const INT64 page_number_mask = 0xffffffffffff8000;
const INT64 offset_mask = 0x7fff;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}


int main(int argc, char *argv[]) {
    check(argc > 1, "Needs an address");

    INT64 decimal_address = atoll(argv[1]);
    
    INT64 page_number = (decimal_address & page_number_mask) >> OFFSET_BITS;
    INT64 offset = decimal_address & offset_mask;

    printf("Page Number: %ld \nOffset: %ld \n", page_number, offset);

    return 0;
}
