#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "charList.h"
#include "stdlib.h"

#define COMMAND_LENGTH 2
#define INPUT_LENGTH_COMMAND COMMAND_LENGTH + 2
#define INT_INPUT_LENGTH 2
#define INPUT_LENGTH_INT INT_INPUT_LENGTH + 2


void checkCommand(char * buffer, int listIndex);
bool checkCommandWithoutIndex(char * buffer);
void getInput(char * buffer, int count);
char * getInputWithValue(int bufferLength);
void printInterface();

int main() {
    initialize();
    printInterface();
    bool finish = false;
    char command[INPUT_LENGTH_COMMAND];
    char intInput[INPUT_LENGTH_INT];
    int listIndex;
    while(!finish){
        printf("----\nEnter a command\n-----\n");
        getInput(command, INPUT_LENGTH_COMMAND);
        printf("\n----\nCommand = %s\n----\n", command);
        if(strcmp(command, "fp") == 0){
            printf("Cleaning up program and lists\n");
            cleanUp();
            finish = true;
        }
        else if(!checkCommandWithoutIndex(command)){
            printf("----\nSelect a list index (Enter some value between 0 to %d)\n-----\n", LIST_MAX_NUM_HEADS);
            getInput(intInput, INPUT_LENGTH_INT);
            listIndex = (int)strtol(intInput, (char**)NULL, 10);
            printf("----\nEntered index = %d\n----\n", listIndex);
            checkCommand(command, listIndex);
        }
    }
}

bool checkCommandWithoutIndex(char * buffer){
    if(strcmp(buffer, "cl") == 0) {
        int index = addNewList();
        printf("New list index = %d\n", index);
        printAllLists();
        return true;
    }
    else if(strcmp(buffer, "pa") == 0) {
        printf("Print all lists\n");
        printAllLists();
        return true;
    }
    else if(strcmp(buffer, "hp") == 0) {
        printInterface();
        return true;
    }
    else {
        return false;
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
    else if(strcmp(buffer,"ap")  == 0){
        printf("Append\n");
        appendCharItem(charList, getInputWithValue(20));
        printList(charList);
    }
    else if(strcmp(buffer,"pp")  == 0){
        printf("Prepend\n");
        prependCharItem(charList, getInputWithValue(20));
        printList(charList);
    }
    else if(strcmp(buffer,"rs")  == 0){
        printf("Remove\n");
        printf("Removed item = %s", removeCharItem(charList));
        printList(charList);
    }
    else if(strcmp(buffer,"tl")  == 0){
        printf("Trim\n");
        printf("Removed item = %s", trimList(charList));
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
    else if(strcmp(buffer, "c2") == 0){
        printf("Concat. Enter a list by index to concat:\n");
        int listForConcat = (int)strtol(getInputWithValue(2), (char**)NULL, 10);
        printf("List for indexing = %d\n", listForConcat);
        if(listForConcat < LIST_MAX_NUM_HEADS && listForConcat > -1 && getListFromIndex(listForConcat) != NULL) {
            concatLists(charList, listForConcat);
        }
        else {
            printf("Unable to concat with given list\n");
        }
        printAllLists();
    }
    else if(strcmp(buffer, "fl") == 0) {
        printf("Free List\n");
        freeList(charList, listIndex);
        printAllLists();
    }
    else if(strcmp(buffer, "sl") == 0) {
        printf("Search. Enter a word to look for:\n");
        char wordToSearch[100];
        getInput(wordToSearch, 100);
        char * foundWord = findItem(charList, wordToSearch);
        printf("Word found = %s\n", foundWord);
        printList(charList);
    }
    else if(strcmp(buffer,"pl")  == 0){
        printf("Print list\n");
        printList(charList);
    }
    else {
        printf("Command does not exist!\n");
    }
}

void getInput(char * buffer, int count) {
    printf("Enter some input:\n");
    fgets(buffer, count, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

char * getInputWithValue(int bufferLength){
    char * string = (char *)malloc(bufferLength);
    getInput(string, bufferLength);
    return string;
}

void printInterface(){
    printf(
        "\n||---Testing List class commands---||\n - as: Add new string \n - is: Insert string \n - pp: Prepend string \n - ap: Append string \n - rs: Remove string at the current pointer \n - tl: Trim list \n - ni: Iterate to next item \n - pi: Iterate to previous item \n - gs: Get current string \n - ss: Get head string \n - es: Get end string \n - cl: Create List \n - c2: Concatenate Lists \n - fl: Free List: \n - sl: Search List\n - pl: Print list \n - pa: Print all Lists \n - fp: Finish program \n ||---Testing List class commands (Enter \"hp\" to show this again!)---||\n\n"
    );
}
