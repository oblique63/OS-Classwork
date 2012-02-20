#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    size_t MAX_LINE = 512;
    const int MAX_ARGS = 100;
    
    char *input, *tokens, *args[MAX_ARGS];
    int input_len, arg_len, arg_count, background = 0;
    int pid;
    
    while (1) {
        printf("Myshell> ");
        getline(&input, &MAX_LINE, stdin);

        input_len = strlen(input)-1;
        input[input_len] = '\0';

        arg_count = 0;
        tokens = strdup(input);

        for (args[arg_count] = strtok(tokens, " ");  args[arg_count];  args[arg_count] = strtok(NULL, " ")) {
            puts(args[arg_count]);
            arg_count++;
        }

        arg_len = strlen(args[arg_count-1]);
        background = args[arg_count-1][arg_len-1] == '&';
        if (background)
            args[arg_count-1][arg_len-1] = '\0';
        
        pid = fork();
        if (pid == -1) {
            perror("Fork failed.");
            exit(-1);
        }
        else if (pid == 0) {
            execvp(args[0], args);
        }
        else if (!background) {
            waitpid(pid, 0, 0);
        }
        
        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
            exit(0);
        }
        
    }
    
    return 0;
}
