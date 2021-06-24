#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "listMonitor.h"

int main(int argc, char *argv[]) {
    char * str1 = (char*)malloc((sizeof(char) * 20));
    strcpy(str1, "Hello world!");
    printf("str1 = %s\n", str1);
    char buf[20];
    strcpy(buf, str1);
    free(str1);
    printf("buf = %s\n", buf);
}