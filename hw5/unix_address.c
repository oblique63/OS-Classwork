#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 32768
#define PAGE_NUMBER_BITS 49
#define OFFSET_BITS 15
#define BYTES 8

typedef unsigned long long int INT64;

// Bit mask to extract the OFFSET_BITS from an address
const INT64 offset_mask = 0x7fff;

void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void decimal_to_octal(int decimal, char* octal) {
    int ascii_offset = 48;
    int octal_digit;
    int digit_count;
    
    int i;
    char temp;

    memset(octal, 0, BYTES);

    // Calculate Octal number, from least to most significant digit
    for (i = 0; decimal > 0 && i < BYTES; i++) {
        octal_digit = decimal % 8;
        octal[i] = octal_digit + ascii_offset;
        decimal /= 8;
    }
    digit_count = i - 1;

    // Reverse Octal string
    for (i = 0; i <= digit_count/2; i++) {
        temp = octal[i];
        octal[i] = octal[digit_count-i];
        octal[digit_count-i] = temp;        
    }
}

int main(int argc, char *argv[]) {
    check(argc > 1, "Needs an address");

    INT64 address = atoll(argv[1]);
    char octal_address[BYTES];
    decimal_to_octal(address, octal_address);
    
    INT64 page_number = address >> OFFSET_BITS;
    char octal_page_number[BYTES];
    decimal_to_octal(page_number, octal_page_number);
    
    INT64 offset = address & offset_mask;
    char octal_offset[BYTES];
    decimal_to_octal(offset, octal_offset);

    printf("Address: \n    Decimal: %ld \n    Octal: %s\n", address, octal_address);
    printf("Page Number: \n    Decimal: %ld \n    Octal: %s\n", page_number, octal_page_number);
    printf("Offset: \n    Decimal: %ld \n    Octal: %s\n", offset, octal_offset);

    return 0;
}
