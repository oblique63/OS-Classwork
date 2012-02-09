/* unix_copy.c
 * Enrique Gavidia
 * CSC 415, Spring 2012
 * Homework 1: File Copy
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define buffer_size  11
#define max_filename 80

// option to print debug statements for smaller input files
int debug_output = 0;

// check if an expression is true, otherwise close the opened files and kill the program
void check(int expression, char *message, int in_fd, int out_fd) {
    if (!expression) {
        perror(message);
        close(in_fd);
        close(out_fd);
        exit(-1);
    }
}

// read user input, check for errors, and remove trailing new-line character from fgets result
void get_input(char *buffer) {
    // and catch the return value of the scanf call in case it fails
    int input = scanf("%s", buffer);
    // check to make sure that 'fgets' didn't throw an error, otherwise exit the program
    check(input != EOF, "[ERROR] Input read failed", -1, -1);
}

void copy_file(char *input_file, char *output_file) {
    // Open the input file with read permissions
    int in_fd = open(input_file, O_RDONLY);
    check(in_fd != -1, "[ERROR] Failed to open input file", in_fd, -1);

    // Open the output file with write permissions (create it if it doesn't already exist),
    // and give the user read/write permissions as well
    int out_fd = open(output_file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    check(out_fd != -1, "[ERROR] Failed to open output file", in_fd, out_fd);

    // ensure the buffer is always null-terminated by setting the last byte permanently to a null-character
    char buffer[buffer_size+1];
    buffer[buffer_size] = '\0';

    int bytes_copied = 0;
    int bytes_read, bytes_written;
    do {
        // clear the buffer to avoid having any unwanted characters from the last read
        memset(buffer, '\0', buffer_size);

        // read in 'buffer_size' bytes of data from the input file and store it in the buffer
        bytes_read = read(in_fd, buffer, buffer_size);
        check(bytes_read != -1, "[ERROR] Failed to read input file", in_fd, out_fd);

        // write the contents of the buffer to the output file, but limit it to the amount of
        // bytes actually read during the read system call
        bytes_written = write(out_fd, buffer, bytes_read);
        check(bytes_written != -1, "[ERROR] Failed to write output file", in_fd, out_fd);

        bytes_copied += bytes_written;

        // print the contents of the buffer
        if (debug_output) {
            puts("\n--------");
            printf("BUFFER: %s\n", buffer);
            puts("--------");
        }
        printf("%s", buffer);

    } while (bytes_read > 0);

    close(in_fd);
    close(out_fd);
    printf("File copy successful, %d bytes copied\n", bytes_copied);
}

int main(int argc, char *argv[]) {
    char input_file[max_filename];
    char output_file[max_filename];

    // Allow a debug flag for displaying debug output
    if (argc > 1 && strcmp(argv[1], "-d") == 0)
        debug_output = 1;
    
    puts("Welcome to the TEE Copy Program by Enrique Gavidia!");
    puts("Enter the name of the file to copy from:");
    get_input(input_file);

    puts("Enter the name of the file to copy to:");
    get_input(output_file);

    copy_file(input_file, output_file);

    return 0;
}
