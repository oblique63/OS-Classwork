/* win_copy.c
 * Enrique Gavidia
 * CSC 415, Spring 2012
 * Homework 1: File Copy
 */

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <windows.h>
#include <Winerror.h>
#include <string.h>

#define buffer_size  11
#define max_filename 80

// for the small input files
#define debug_output 0

// check if an expression is true, otherwise close the opened files and kill the program
void check(int expression, char *message, HANDLE in_handle, HANDLE out_handle, int return_code) {
    if (!expression) {
        printf("%s: %x\n", message, GetLastError());
        CloseHandle(in_handle);
        CloseHandle(out_handle);
        exit(return_code);
    }
}

// read user input, check for errors, and remove trailing new-line character from fgets result
void get_input(char *buffer) {
    // and catch the return value of the scanf call in case it fails
    int input = scanf("%s", buffer);
    // check to make sure that 'fgets' didn't throw an error, otherwise exit the program
    check(input != EOF, "[ERROR] Input read failed", NULL, NULL, ERROR_READ_FAULT);
}


void copy_file(char *input_file, char *output_file) {
    HANDLE in_handle, out_handle;
    DWORD bytes_read, bytes_written, bytes_copied;
    BOOL syscall_success;
    
    char buffer[buffer_size+1];
    
    // Open the input file with read permissions
    in_handle = CreateFile(input_file, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    check(in_handle != INVALID_HANDLE_VALUE, "[ERROR] Failed to open input file", in_handle, NULL, ERROR_FILE_NOT_FOUND);

    // Open the output file with write permissions (create it if it doesn't already exist),
    // and give the user read/write permissions as well
    out_handle = CreateFile(input_file, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
    check(out_handle != INVALID_HANDLE_VALUE, "[ERROR] Failed to open output file", in_handle, out_handle, ERROR_CANNOT_MAKE);
    
    // ensure the buffer is always null-terminated by setting the last byte permanently to a null-character
    buffer[buffer_size] = '\0';
    do {
        // clear the buffer to avoid having any unwanted characters from the last read
        memset(buffer, '\0', buffer_size);

        // read in 'buffer_size' bytes of data from the input file and store it in the buffer
        syscall_success = ReadFile(in_handle, buffer, buffer_size, &bytes_read, NULL);
        check(syscall_success != FALSE, "[ERROR] Failed to read input file", in_handle, out_handle, ERROR_READ_FAULT);
        printf("READ %d bytes\n", bytes_read);
        printf("BUFFER: %s\n", buffer);
        
        // write the contents of the buffer to the output file, but limit it to the amount of
        // bytes actually read during the read system call
        syscall_success = WriteFile(out_handle, buffer, bytes_read, &bytes_written, NULL);
        check(syscall_success != FALSE, "[ERROR] Failed to write output file", in_handle, out_handle, ERROR_WRITE_FAULT);
        printf("WROTE %d bytes\n", bytes_written);
        bytes_copied += bytes_written;

        // print the contents of the buffer
        if (debug_output) {
            puts("--------");
            printf("\nBUFFER: %s\n", buffer);
        }
        printf("%s", buffer);

    } while (bytes_read > 0);

    CloseHandle(in_handle);
    CloseHandle(out_handle);
    printf("File copy successful, %d bytes copied\n", bytes_copied);
}


int main() {
    char input_file[max_filename];
    char output_file[max_filename];

    puts("Welcome to the TEE Copy Program by Enrique Gavidia!");
    puts("Enter the name of the file to copy from:");
    get_input(input_file);

    puts("Enter the name of the file to copy to:");
    get_input(output_file);

    copy_file(input_file, output_file);

    return 0;
}
