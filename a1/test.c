#include <stdio.h>
#include "list.h"
#include "charList.h"

void getInput(char * buffer, int count);
void printInterface();

int main() {
    initialize();
    printList();
    printInterface();
}

void getInput(char * buffer, int count) {
    printf("Enter some input:\n");
    fgets(buffer, count, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void printInterface(){
    printf(
        "||---Testing List class commands...---||\n - as: Add new string \n - is: Insert string \n - rs: Remove string at the current pointer \n - ni: Iterate to next item \n - pi: Iterate to previous item \n - gs: Get current string \n - ss: Get head string \n - es: Get end string \n - pl: Print list \n||---Testing List class commands...---||\n"
    );
}
