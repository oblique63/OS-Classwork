#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROCESSES 10

// Ensure that an expression is true, otherwise safely exit the program
void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

int main() {
    int i, j, pid;

    for (i=1; i <= PROCESSES; i++) {
        pid = fork();
        check(pid != -1, "Fork Failed");

        if (pid == 0) {
            pid = getpid();
            for (j=1; j <= 10; j++) {
                fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", i, pid, j);
                sleep(1);
            }
            exit(0);
        }
    }

    waitpid(pid, 0, 0);
    return 0;
}
