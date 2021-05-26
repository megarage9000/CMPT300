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
    printf("charList info: \n - count = %d \n - status = %d\n - current item = %s\n", 
            charList->count, charList->status, getCurrentItem());
    char * tailItem = getTailItem();
    char * headItem = getHeadItem();
    if(tailItem != NULL && headItem != NULL){
        for(int i = 0; charList->status != LIST_OOB_END; ++i){
            printf("\t item #%d = %s", i, getCurrentItem());
            nextItem();
        }
    }
}
