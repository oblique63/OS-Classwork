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
    int i, pid, child_status;
    char pid_str[100], temp[100];

    for (i=1; i <= PROCESSES; i++) {
        pid = fork();
        check(pid != -1, "Fork Failed");

        if (pid == 0) {
            sleep(1);
            exit(0);
        }
        else {
            if (i > 1)
                strcpy(temp, pid_str);
            
            memset(pid_str, '\0', 100);
            sprintf(pid_str, "%s%d: %d\n", temp, i, pid);
        }
    }
    
    write(2, pid_str, sizeof(pid_str));
    
    return 0;
}
