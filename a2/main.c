#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buf[100];
    char printMessage[] = "Enter a message\n";
    while(1) {
        while (read(STDIN_FILENO, buf, sizeof buf) > 0) {
            write(STDOUT_FILENO, printMessage, sizeof printMessage);
            buf[100] = '\0';
            printf("You entered = %s\n", buf);
        }
    }
}