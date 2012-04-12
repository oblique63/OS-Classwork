#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long int INT64;

INT64 PAGE_SIZE = 32768;
int BYTES = 8;
int OFFSET_BITS = 15;
int PAGE_NUMBER_BITS = 49;

// Bit mask to extract the OFFSET_BITS from an address
INT64 OFFSET_MASK = 0x7fff;


void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void use_page_size(int page_size_in_kb) {
    PAGE_SIZE = page_size_in_kb * 1024;
    BYTES = PAGE_SIZE / 8;
    OFFSET_BITS = roundl(log2(PAGE_SIZE));
    OFFSET_MASK = pow(2, OFFSET_BITS) - 1;
    PAGE_NUMBER_BITS = 64 - OFFSET_BITS;
}

void decimal_to_octal(int decimal, char* octal) {
    int octal_digit, digit_count;

    int i;
    char temp;

    memset(octal, 0, BYTES);

    // Calculate Octal number, from least to most significant digit
    for (i = 0; decimal > 0 && i < BYTES; i++) {
        octal_digit = decimal % 8;
        octal[i] = octal_digit + '0';  // converts to ASCII digit
        decimal /= 8;
    }

    digit_count = i - 1;

    // Reverse Octal string
    for (i = 0; i < digit_count/2; i++) {
        temp = octal[i];
        octal[i] = octal[digit_count-i];
        octal[digit_count-i] = temp;
    }
}

int main(int argc, char *argv[]) {
    INT64 address, page_number, offset;
    char *octal_address, *octal_page_number, *octal_offset;

    check(argc > 1, "Needs an address");

    if (argc > 2)
        use_page_size(atoi(argv[2]));

    address = atoll(argv[1]);
    octal_address = calloc(BYTES, sizeof(char));
    decimal_to_octal(address, octal_address);

    page_number = address >> OFFSET_BITS;
    octal_page_number = calloc(BYTES, sizeof(char));
    decimal_to_octal(page_number, octal_page_number);

    offset = address & OFFSET_MASK;
    octal_offset = calloc(BYTES, sizeof(char));
    decimal_to_octal(offset, octal_offset);

    printf("Address: \n    Decimal: %ld \n    Octal: %s\n", address, octal_address);
    printf("Page Number: \n    Decimal: %ld \n    Octal: %s\n", page_number, octal_page_number);
    printf("Offset: \n    Decimal: %ld \n    Octal: %s\n", offset, octal_offset);

    free(octal_address);
    free(octal_page_number);
    free(octal_offset);
    
    return 0;
}
