#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 100

// Ensure that an expression is true, otherwise safely exit the program
void check(int expression, char *message) {
    if (!expression) {
        perror(message);
        exit(-1);
    }
}

int main() {
    size_t max_input = 0;
    char *input, *tokens, *args[MAX_ARGS];
    int input_len, arg_len, arg_count, pid, child_status, return_code;
    int background = 0;

    while (1) {
        printf("Myshell> ");
        getline(&input, &max_input, stdin);

        // Remove the trailing '\n' character from the input
        input_len = strlen(input)-1;
        input[input_len] = '\0';

        // Don't bother creating any processes if the user didn't enter anything
        if (input_len > 0) {
            arg_count = 0;
            tokens = strdup(input);

            for (args[arg_count]=strtok(tokens, " ");  args[arg_count];  args[arg_count]=strtok(NULL, " ")) {
                arg_count++;
            }

            // Check if the user requested the command to be executed in the background
            arg_len = strlen(args[arg_count-1]);
            background = args[arg_count-1][arg_len-1] == '&';
            // If it is a background process, remove the '&' from the arguments to ensure normal execution
            if (background)
                args[arg_count-1][arg_len-1] = '\0';

            if (strcasecmp(args[0], "exit") == 0 || strcasecmp(args[0], "quit") == 0) {
                exit(0);
            }

            pid = fork();
            check(pid != -1, "Fork failed");

            if (pid == 0) {
                // In the child process: execute the requested command with its arguments
                return_code = execvp(args[0], args);
                check(return_code != -1, "ERROR");
                exit(0);
            }

            else if (background) {
                // If the child is set to be a background process, don't halt the execution of the parent ('WNOHANG')
                waitpid(WAIT_ANY, &child_status, WNOHANG);
            }

            else {
                // If the child is not in the background, wait until it finishes its execution to display the prompt again
                return_code = waitpid(pid, &child_status, 0);
                check(return_code != -1, "Wait for child process failed");
            }
        }
    }

    return 0;
}
