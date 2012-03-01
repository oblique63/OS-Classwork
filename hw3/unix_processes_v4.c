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
    char *temp;

    for (i=1; i <= PROCESSES; i++) {
        pid = fork();
        check(pid != -1, "Fork Failed");

        if (pid == 0) {
            pid = getpid();
            
            sprintf(temp, "%d: %d\n",  i, pid);
            fputs(temp, stderr);
            
            sleep(1);
            exit(0);
        }
    }

    //waitpid(pid, &child_status, 0);
    //fputs(pid_str, stderr);
    
    return 0;
}
