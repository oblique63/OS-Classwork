#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define buffer_size 11


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
    // read input string, limiting the size of it to 'buffer_size',
    // and catch the return value of the fgets call in case it fails
    char *input = fgets(buffer, sizeof(char) * buffer_size, stdin);

    // check to make sure that 'fgets' didn't throw an error, otherwise exit the program
    check(input != NULL, "Input read failed", -1, -1);

    // remove the new-line character from the string
    int length = strlen(buffer);
    if (buffer[length-1] == '\n')
        buffer[length-1] = '\0';
}


void copy_file(char *input_file, char *output_file) {
    // Open the input file with read permissions
    int in_fd = open(input_file, O_RDONLY);
    check(in_fd != -1, "Failed to open input file", in_fd, -1);

    // Open the output file with write permissions (create it if it doesn't already exist),
    // and give the user read/write permissions as well
    int out_fd = open(output_file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    check(out_fd != -1, "Failed to open output file", in_fd, out_fd);

    char buffer[buffer_size];
    int bytes_read, bytes_written;
    do {
        // clear the buffer to avoid having any unwanted characters from the last read
        memset(buffer, '\0', buffer_size);

        // read in 'buffer_size' bytes of data from the input file and store it in the buffer
        bytes_read = read(in_fd, buffer, buffer_size);
        check(bytes_read != -1, "Failed to read input file", in_fd, out_fd);

        // write the contents of the buffer to the output file, but limit it to the amount of
        // bytes actually read during the read system call
        bytes_written = write(out_fd, buffer, bytes_read);
        check(bytes_written != -1, "Failed to write output file", in_fd, out_fd);
        
        // print the contents of the buffer
        printf("%s\0", buffer);

    } while (bytes_read > 0);

    close(in_fd);
    close(out_fd);
}


int main(int argc, char *argv[]) {
    
    char input_file[buffer_size];
    char output_file[buffer_size];

    puts("Welcome to the TEE Copy Program by Enrique Gavidia!");
    puts("Enter the name of the file to copy from:");
    //get_input(input_file);
    scanf("%s", input_file);
    
    puts("Enter the name of the file to copy to:");
    //get_input(output_file);
    scanf("%s", output_file);

    copy_file(input_file, output_file);

    return 0;
}
