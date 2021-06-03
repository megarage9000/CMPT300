#include "charList.h"

void initialize() {
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        charLists[i] = NULL;
    }
}

List * createCharList(){
    return List_create();
}

int addNewList(){
    List * list = createCharList();
    if(list != NULL){
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
            if(charLists[i] == NULL) {
                charLists[i] = list;
                return i;
            }
        }
    }   
    return -1;
}

List * getListFromIndex(int index){
    return charLists[index];
}

void addCharItem(List * charList, char * item){
    List_add(charList, (void *)item);
}

void insertCharItem(List * charList, char * item){
    List_insert(charList, (void *)item);
}

char * removeCharItem(List * charList){
    return (char *)List_remove(charList);
}

char * nextItem(List * charList){
    return (char *)List_next(charList);
}

char * prevItem(List * charList){
    return (char *)List_prev(charList);
}

char * getCurrentItem(List * charList){
    return (char *)List_curr(charList);
}

char * getTailItem(List * charList){
    return (char *)List_last(charList);
}

char * getHeadItem(List * charList){
    return (char *)List_first(charList);
}

char * trim(List * charList){
    return (char *)List_trim(charList);
}

void concatLists(List * charList1, int listIndex){
    List_concat(charList1, getListFromIndex(listIndex));
    charLists[listIndex] = NULL;
}

void freeItem(void * item){
    free(item);
}

void freeList(List * charList, int listIndex){
    List_free(charList, freeItem);
    charLists[listIndex] = NULL;
}

bool checkMatchingChars(void * item1, void * item2){
    return (strcmp((char *)item1, (char *)item2) == 0);
}

char * findItem(List * charList, char * matchingString){
    return (char *)List_search(charList, checkMatchingChars, matchingString);
}

void printList(List * charList) {
    printf("---------------------------\n");
    printf("charList info: \n - count = %d \n - status = %d\n - current item = %s\n", 
            charList->count, charList->status, getCurrentItem(charList));
    Node * startingNode = charList->head;
    Node * head = startingNode;
    Node * tail = charList->tail;
    printf("\n");
    for(int i = 0; startingNode != NULL; i++){
        printf("\t%dth item ", i);
        printNodeInfo(startingNode);
        printf("\n");
        startingNode = startingNode->next;
    }

    if(head != NULL && tail != NULL){
        printf(" - (Head node) ");
        printNodeInfo(head);
        printf(" - (Tail node) ");
        printNodeInfo(tail);
    }
    
    printf("---------------------------\n");
}

void printAllLists() {
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List * list = charLists[i];
        if(list != NULL){
            printf("List index %d: \n", i);
            printList(list);
        }
    }
}

void printNodeInfo(Node * node){
    printf("Node information: prev: %p, next: %p, item: %s\n", node->prev, node->next, (char *)node->item);
}

void cleanUp(){
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        List * list = charLists[i];
        if(list != NULL){
            freeList(list, i);
        }
    }
}