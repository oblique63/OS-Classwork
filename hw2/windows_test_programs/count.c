#include <stdio.h>

int main(int argc, char *argv[]) {

    int upto;
    int i;
    
    if (argc < 2) {
        puts("Must provide a number to count up to.");
        return -1;
    }

    upto = atoi(argv[1]);
    for (i=1; i <= upto; i++)
        printf("%d\n", i);
    
    return 0;
}
