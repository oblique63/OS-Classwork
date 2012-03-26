#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define PROCESSES 10

int process_index = 0;
int pids[PROCESSES];

// Ensure that an expression is true, otherwise safely exit the program
void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

void print_values() {
    //if (process_index < PROCESSES-1)
    //    signal(SIGHUP, print_values);
    
    int pid = getpid();
    int j = 0;
    for (j=1; j <= 10; j++) {
        fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", process_index+1, pid, j);
        sleep(1);
    }

    // Send a signal to the next process to print out its values,
    // unless this is already the last process
    if (process_index < PROCESSES-1)
        kill(pids[process_index+1], SIGHUP);

    exit(0);
}


int main() {
    int i;
    for (i=PROCESSES-1; i >= 0; i--) {
        pids[i] = fork();
        check(pids[i] != -1, "Fork Failed");

        // prevent forking more processes if this is a child
        if (pids[i] == 0)
            break;
    }

    // Parent
    if (i < 0)
        waitpid(pids[PROCESSES-1], 0, NULL);

    // Child
    else {
        process_index = i;
        
        if (process_index == 0) {
            // Execute the first process as soon as it's ready
            print_values();
        }
        
        else {
            // Stall the other child processes until they
            // recieve the signal to print out their values
            signal(SIGHUP, print_values);
            while(1);
        }
    }
    
    return 0;
}
