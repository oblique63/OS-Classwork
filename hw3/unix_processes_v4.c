#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESSES 10

// Ensure that an expression is true, otherwise safely exit the program
void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

int main() {
    int i, pid;
    char *output, *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";;

    for (i=1; i <= PROCESSES; i++) {
        pid = fork();
        check(pid != -1, "Fork Failed");

        if (pid == 0) {
            pid = getpid();

            // Build the string, then output the completed string with fputs
            asprintf(&output, "[Process #%d] \t ID: %d \t Values:\n%s\n",  i, pid, values);
            fputs(output, stderr);

            sleep(1);
            exit(0);
        }
    }

    return 0;
}
