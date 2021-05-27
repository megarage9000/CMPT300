#include <stdio.h>
#include <string.h>
#include "list.h"
#include "charList.h"
#include "stdlib.h"


void checkCommand(char * buffer);
void getInput(char * buffer, int count);
char * getInputWithValue(int bufferLength);
void printInterface();

int main() {
    initialize();
    int finish = 0;
    char command[2];
    while(!finish){
        printInterface();
        getInput(command, 2);
        printf("----\nCommand = %s\n-----\n", command);
        checkCommand(command);
    }
}

void checkCommand(char * buffer){
    if(strcmp(buffer,"as")  == 0){
        printf("Add\n");
        addCharItem(getInputWithValue(20));
    }
    else if(strcmp(buffer,"is")  == 0){
        printf("Insert\n");
        insertCharItem(getInputWithValue(20));
    }
    else if(strcmp(buffer,"rs")  == 0){
        printf("Remove\n");
        printf("Removed item = %s", removeCharItem());
    }
    else if(strcmp(buffer,"ni")  == 0){
        printf("Next\n");
        nextItem();
    }
    else if(strcmp(buffer,"pi")  == 0){
        printf("Previous\n");
        prevItem();
    }
    else if(strcmp(buffer,"gs")  == 0){
        printf("Current\n");
        printf("Current item = %s\n", getCurrentItem());
    }
    else if(strcmp(buffer,"ss")  == 0){
        printf("Start\n");
        printf("Head item = %s\n", getHeadItem());
    }
    else if(strcmp(buffer,"es")  == 0){
        printf("End\n");
        printf("Tail item = %s\n", getTailItem());
    }
    else if(strcmp(buffer,"pl")  == 0){
        printf("Print list\n");
        printList();
    }
}

void getInput(char * buffer, int count) {
    printf("Enter some input:\n");
    fgets(buffer, count + 2, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

char * getInputWithValue(int bufferLength){
    char * string = (char *)malloc(bufferLength);
    getInput(string, bufferLength);
    return string;
}

void printInterface(){
    printf(
        "\n||---Testing List class commands---||\n - as: Add new string \n - is: Insert string \n - rs: Remove string at the current pointer \n - ni: Iterate to next item \n - pi: Iterate to previous item \n - gs: Get current string \n - ss: Get head string \n - es: Get end string \n - pl: Print list \n||---Testing List class commands---||\n\n"
    );
}
