#include <stdio.h>
#include <windows.h>

#define PROCESSES 10

void close_process(PROCESS_INFORMATION process) {
    CloseHandle(process.hProcess);
    CloseHandle(process.hThread);
}

void check(int expression, char *message, PROCESS_INFORMATION process) {
    if (!expression) {
        DWORD error = GetLastError();
        fprintf(stderr, "%s: %x\n", message, error);
        close_process(process);
        exit(error);
    }
}

int main(int argc, char *argv[]) {
    // The first argument is the implementation version number that corresponds to the function to be
    // executed in the 'win_processes' program
    int version = atoi(argv[1]);
    
    int i, j, pid;
    int success = 0;
    DWORD return_code;
    STARTUPINFO startup_info;
    PROCESS_INFORMATION processes[PROCESSES];
    
    char path[256];
    char args[256];

    GetStartupInfo(&startup_info);

    // the 'win_processes' program handles all the printing of values for all the versions
    _getcwd(&path, 256);
    strcat(path, "\\win_processes.exe" );
    
    for (i=0; i < PROCESSES; i++) {
        memset(args, '\0', sizeof(args));
        sprintf(args, "%s %d %d", path, version, i);
        
        success = CreateProcess(path, args,
                                NULL, NULL,
                                TRUE, 0,
                                NULL, NULL,
                                &startup_info, &processes[i]);

        check(success != NULL, "Failed to create process", processes[i]);
    }

    for (i=0; i < PROCESSES; i++) {
        WaitForSingleObject(processes[i].hProcess, INFINITE);
        close_process(processes[i]);
    }

    return 0;
}
