#include <stdio.h>
#include <string.h>
#include "list.h"
#include "charList.h"
#include "stdlib.h"


void checkCommand(char * buffer, int listIndex);
void getInput(char * buffer, int count);
char * getInputWithValue(int bufferLength);
void printInterface();

int main() {
    initialize();
    printInterface();
    int finish = 0;
    char command[2];
    int listIndex;
    while(!finish){
        printf("----\nEnter a command\n-----\n");
        getInput(command, 2);
        printf("----\nCommand = %s, select a list index (Enter some value between 0 to %d)\n-----\n", command, LIST_MAX_NUM_HEADS);
        listIndex = (int)strtol(getInputWithValue(2), (char**)NULL, 10);
        checkCommand(command, listIndex);
    }
}

void checkCommand(char * buffer, int listIndex){
    if(listIndex > LIST_MAX_NUM_HEADS || listIndex < 0){
        printf("Index out of bounds\n");
        return;
    }

    List * charList = getListFromIndex(listIndex);

    if(charList == NULL){
        printf("List does not exist!\n");
        return;
    }
    else if(strcmp(buffer,"as")  == 0){
        printf("Add\n");
        addCharItem(charList, getInputWithValue(20));
        printList(charList);
    }
    else if(strcmp(buffer,"is")  == 0){
        printf("Insert\n");
        insertCharItem(charList, getInputWithValue(20));
        printList(charList);
    }
    else if(strcmp(buffer,"rs")  == 0){
        printf("Remove\n");
        printf("Removed item = %s", removeCharItem(charList));
        printList(charList);
    }
    else if(strcmp(buffer,"ni")  == 0){
        printf("Next\n");
        nextItem(charList);
        printList(charList);
    }
    else if(strcmp(buffer,"pi")  == 0){
        printf("Previous\n");
        prevItem(charList);
        printList(charList);
    }
    else if(strcmp(buffer,"gs")  == 0){
        printf("Current\n");
        printf("Current item = %s\n", getCurrentItem(charList));
        printList(charList);
    }
    else if(strcmp(buffer,"ss")  == 0){
        printf("Start\n");
        printf("Head item = %s\n", getHeadItem(charList));
        printList(charList);
    }
    else if(strcmp(buffer,"es")  == 0){
        printf("End\n");
        printf("Tail item = %s\n", getTailItem(charList));
        printList(charList);
    }
    else if(strcmp(buffer,"pl")  == 0){
        printf("Print list\n");
        printList(charList);
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
