#include <stdio.h>
#include <windows.h>

void version1(DWORD pid, int process_num) {
    int j;
    for (j=1; j <= 10; j++)
        fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", process_num, pid, j);
}

void version2(DWORD pid, int process_num) {
    int j;
    for (j=1; j <= 10; j++) {
        fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", process_num, pid, j);
        Sleep(1);
    }
}

void version3(DWORD pid, int process_num) {
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";

    fprintf(stderr, "[Process #%d] \t ID: %d \t Values:\n%s\n", process_num, pid, values);
    Sleep(1);
}

void version4(DWORD pid, int process_num) {
    char *output;
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";

    sprintf(output, "[Process #%d] \t ID: %d \t Values:\n%s\n",  process_num, pid, values);
    fputs(output, stderr);
    Sleep(1);
}

void version5(DWORD pid, int process_num) {
    char output[256];
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";
    int output_length;

    memset(output, '\0', 256);
    sprintf(output, "[Process #%d] \t ID: %d \t Values:\n%s\n",  process_num, pid, values);
    
    WriteFile(GetStdHandle(STD_ERROR_HANDLE), &output, strlen(output), NULL, NULL);
    Sleep(1);
}

int main(int argc, char *argv[]) {
    int version = atoi(argv[1]);
    int iteration = atoi(argv[2])+1;
    DWORD pid = GetCurrentProcessId();
    
    if (version == 1)
        version1(pid, iteration);
    else if (version == 2)
        version2(pid, iteration);
    else if (version == 3)
        version3(pid, iteration);
    else if (version == 4)
        version4(pid, iteration);
    else if (version == 5)
        version5(pid, iteration);
    
    return 0;
}
