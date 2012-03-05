#include <stdio.h>
#include <windows.h>

void version1(DWORD pid, int iteration) {
    int j;
    for (j=1; j <= 10; j++)
        fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", iteration+1, pid, j);
}

void version2(DWORD pid, int iteration) {
    int j;
    for (j=1; j <= 10; j++) {
        fprintf(stderr, "[Process #%d] \t ID: %d \t Value: %d\n", iteration+1, pid, j);
        Sleep(1);
    }
}

void version3(DWORD pid, int iteration) {
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";

    fprintf(stderr, "[Process #%d] \t ID: %d \t Values:\n%s\n", iteration+1, pid, values);
    Sleep(1);
}

void version4(DWORD pid, int iteration) {
    char *output;
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";

    asprintf(&output, "[Process #%d] \t ID: %d \t Values:\n%s\n",  iteration+1, pid, values);
    fputs(output, STD_ERROR_HANDLE);
    Sleep(1);
}

void version5(DWORD pid, int iteration) {
    char output[256];
    char *values = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";

    sprintf(output, "[Process #%d] \t ID: %d \t Values:\n%s\n",  iteration+1, pid, values);
    WriteFile(GetStdHandle(STD_ERROR_HANDLE), &output, sizeof(output), NULL, NULL);
    
    Sleep(1);
}

int main(int argc, char *argv[]) {
    int version = atoi(argv[1]);
    int iteration = atoi(argv[2]);
    DWORD pid = GetProcessID();
    
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
