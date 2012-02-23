#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 100

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

        input_len = strlen(input)-1;
        input[input_len] = '\0';

        if (input_len > 0) {

            arg_count = 0;
            tokens = strdup(input);
            
            for (args[arg_count]=strtok(tokens, " ");  args[arg_count];  args[arg_count]=strtok(NULL, " ")) {
                arg_count++;
            }

            arg_len = strlen(args[arg_count-1]);
            background = args[arg_count-1][arg_len-1] == '&';
            if (background)
                args[arg_count-1][arg_len-1] = '\0';

            
            if (strcasecmp(args[0], "exit") == 0 || strcasecmp(args[0], "quit") == 0) {
                exit(0);
            }


            pid = fork();
            check(pid != -1, "Fork failed");
            
            if (pid == 0) {
                return_code = execvp(args[0], args);
                check(return_code != -1, "ERROR");
                exit(0);
            }

            else if (background) {
                waitpid(WAIT_ANY, &child_status, WNOHANG | WUNTRACED);
            }

            else {
                waitpid(pid, &child_status, 0);
            }

        }
        
    }

    return 0;
}
