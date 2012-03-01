#include <stdio.h>
#include <stdlib.h>
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
    int i, pid, child_status;

    for (i=1; i <= PROCESSES; i++) {
        pid = fork();
        check(pid != -1, "Fork Failed");

        if (pid == 0) {
            pid = getpid();
            fprintf(stderr, "%d: %d\n", i, pid);
            exit(0);
        }
        else {
            waitpid(pid, &child_status, 0);
        }
        
    }

    return 0;
}