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
    int i, child_status;
    int pids[PROCESSES];

    for (i=0; i < PROCESSES; i++) {
        pids[i] = fork();
        check(pids[i] != -1, "Fork Failed");

        if (pids[i] == 0) {
            pids[i] = getpid();
            sleep(1);
            exit(0);
        }
    }

    waitpid(pids[PROCESSES-1], &child_status, 0);
    fprintf(stderr, "1: %d  \n2: %d  \n3: %d  \n4: %d  \n5: %d  \n6: %d  \n7: %d  \n8: %d  \n9: %d  \n10: %d  \n",
                     pids[0], pids[1], pids[2], pids[3], pids[4], pids[5], pids[6], pids[7], pids[8], pids[9]);
    
    return 0;
}
