#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <direct.h>


void check(int expression, char *message, int return_code, PROCESS_INFORMATION process) {
    if (!expression) {
        puts(message);
        exit_shell(return_code, process);
    }
}

void exit_shell(int return_code, PROCESS_INFORMATION process) {
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
    exit(return_code);
}

int main() {

    size_t max_line = 0;
    const int MAX_ARGS = 100;

    STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;

    char *input, *tokens, *command, *path, *command_path;
    int input_len, cmd_len;
    int background = 0, success = 0;

    // current directory will not change
    path = _getcwd(NULL, 0);

    // setup the startup info
    GetStartupInfo(&startup_info);
    startup_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startup_info.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    startup_info.hStdInput = ReadHandle;
    startup_info.dwFlags = STARTF_USESTDHANDLES;

    

    while (1) {
        printf("Myshell> ");
        getline(&input, &max_line, stdin);

        input_len = strlen(input)-1;
        // remove '\n' character
        input[input_len] = '\0';

        if (input_len > 0) {
            tokens = strdup(input);
            command = strtok(tokens, " ");

            cmd_len = strlen(command);
            background = command[cmd_len-1] == '&';
            if (background)
                command[cmd_len-1] = '\0';


            if (strcasecmp(command, "exit") == 0 || strcasecmp(command, "quit") == 0) {
                exit_shell(0, process_info);
            }

            command_path = path;
            strcat(command_path, command);

            SecureZeroMemory(&process_info, sizeof(process_info));

            success = CreateProcess(command_path, input,
                                        NULL, NULL,
                                        TRUE, 0, //Process Creation Flag?
                                        NULL, NULL,
                                        &startup_info,
                                        &process_info
                                        );

            check(success, "CreateProcess Failed", -1, process_info);

            if (!background)
                WaitForSingleObject(process_info.hProcess, INFINITE);
        }
    }

    return 0;
}
