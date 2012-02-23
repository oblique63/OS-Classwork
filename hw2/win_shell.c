#include <stdio.h>
#include <windows.h>

#define MAX_BUFFER 512

// Make sure to properly end the processes when exiting
void exit_shell(int return_code, PROCESS_INFORMATION process) {
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
    exit(return_code);
}

void check(int expression, char *message, int return_code, PROCESS_INFORMATION process) {
    if (!expression) {
        fprintf(stderr, "%s: %x\n", message, GetLastError());
        exit_shell(return_code, process);
    }
}

int main() {
    // The current directory will not change.
    // Must append a '\' character to the path, because it is a directory.
    const char *PATH = strcat( _getcwd(NULL, 0), "\\" );
    
    char input[MAX_BUFFER], *tokens, *command, command_path[MAX_BUFFER], *has_exe;
    int input_len, cmd_len;
    int background = 0, success = 0;

    DWORD return_code;
    STARTUPINFO startup_info;
    PROCESS_INFORMATION process_info;

    GetStartupInfo(&startup_info);
    
    while (1) {
        printf("Myshell> ");
        fgets(input, MAX_BUFFER, stdin);

        // Remove the '\n' character from the input
        input_len = strlen(input)-1;
        input[input_len] = '\0';

        if (input_len > 0) {
            // Only need to tokenize the first argument (the command);
            // CreateProcess accepts the rest of the arguments as a single string.
            tokens = strdup(input);
            command = strtok(tokens, " ");

            cmd_len = strlen(command);
            background = command[cmd_len-1] == '&';
            if (background)
                command[cmd_len-1] = '\0';

            
            if (_stricmp(command, "exit") == 0 || _stricmp(command, "quit") == 0) {
                exit_shell(0, process_info);
            }

            // A link to the windows 'tasklist' command to easily display currently running tasks
            else if (stricmp(command, "tasklist") == 0) {
                strcpy(command_path, "c:\\windows\\system32\\tasklist.exe");
            }
            
            else {
                // Concatiname the given command, with the current directory path to get
                // full path name of the command.
                strcpy(command_path, PATH);
                strcat(command_path, command);
                
                // Accept commands without an explicit '.exe' extension
                has_exe = strstr(command_path, ".exe");
                if (!has_exe)
                    strcat(command_path, ".exe");
            }
            
            success = CreateProcess(command_path, input,
                                    NULL, NULL,
                                    TRUE, 0,
                                    NULL, NULL,
                                    &startup_info, &process_info);

            if (!success) {
                fprintf(stderr, "Could not execute file '%s.exe'.\n", command);
            }
            
            else if (!background) {
                return_code = WaitForSingleObject(process_info.hProcess, INFINITE);
                check(return_code != WAIT_FAILED, "Wait has failed", WAIT_FAILED, process_info);
            }
        }
    }
    
    exit_shell(0, process_info);
    return 0;
}
