#include "charList.h"

void initialize() {
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
        charLists[i] = NULL;
    }
}

List * createCharList(){
    return List_create();
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

void concatLists(List * charList1, List * charList2){
    List_concat(charList1, charList2);
}

void freeItem(void * item){
    free(item);
}

void freeList(List * charList, int index){
    List_free(charList, freeItem);
}

bool checkMatchingChars(void * item1, void * item2){
    return strcmp((char *)item1, (char *)item2);
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

void printNodeInfo(Node * node){
    printf("Node information: prev: %p, next: %p, item: %s\n", node->prev, node->next, (char *)node->item);
}
