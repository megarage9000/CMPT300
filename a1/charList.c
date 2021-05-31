#include "charList.h"

void initialize(){
    if(charList == NULL){
        charList = List_create();
    }
}

void addCharItem(char * item){
    List_add(charList, (void *)item);
}

void insertCharItem(char * item){
    List_insert(charList, (void *)item);
}

char * removeCharItem(){
    return (char *)List_remove(charList);
}

char * nextItem(){
    return (char *)List_next(charList);
}

char * prevItem(){
    return (char *)List_prev(charList);
}

char * getCurrentItem(){
    return (char *)List_curr(charList);
}

char * getTailItem(){
    return (char *)List_last(charList);
}

char * getHeadItem(){
    return (char *)List_first(charList);
}

void printList() {
    printf("---------------------------\n");
    printf("charList info: \n - count = %d \n - status = %d\n - current item = %s\n", 
            charList->count, charList->status, getCurrentItem());
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
