#include <stdio.h>
#include <windows.h>

#define PROCESSES 10

// Make sure to properly end the processes when exiting
void close_process(PROCESS_INFORMATION process) {
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
}

void check(int expression, char *message, PROCESS_INFORMATION process) {
    if (!expression) {
        fprintf(stderr, "%s: %x\n", message, GetLastError());
        close_process(process);
        exit(return_code);
    }
}

int main(int argc, char *argv[]) {

    int version = atoi(argv[1]);
    
    int i, j, pid;
    int success = 0;
    DWORD return_code;
    STARTUPINFO startup_info;
    PROCESS_INFORMATION processes[PROCESSES];

    const char *path = strcat( _getcwd(NULL, 0), "\\win_processes.exe" );
    char args[256];

    for (i=0; i < PROCESSES; i++) {
        memset(args, '\0', sizeof(args));
        // the 'win_processes.exe' program handles all the printing of values
        sprintf(args, "%s %d %d", path, version, i);
        
        success = CreateProcess(path, args,
                                NULL, NULL,
                                TRUE, 0,
                                NULL, NULL,
                                &startup_info, &process[i]);

        check(success, "Failed to create process.", processes[i]);
    }

    for (i=0; i < PROCESSES; i++) {
        WaitForSingleObject(processes[i].hProcess, INFINITE);
        close_process(processes[i]);
    }

    return 0;
}
