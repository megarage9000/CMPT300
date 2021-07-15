#include <stdio.h>
#include "processes.h"


void getUserInput(char * buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin);
    buffer[bufferSize] = '\0';
}

int main() {
    char buf[20];
    printf("Enter some input: ");
    getUserInput(buf, sizeof(buf)/sizeof(char));
    printf("\nYou entered: %s\n", buf);
}

