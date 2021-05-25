#include <stdio.h>
#include "list.h"

void printListInfo(List * list);

int main() {
    List * list = List_create();
   
   printListInfo(list);
}

void printListInfo(List * list) {
    int count = List_count(list);
    printf("List information:\n - Address: %p\n - Count: %d\n - Head: %p\n - Tail: %p\n - Current: %p\n - Status: %d\n", 
            (List *)list, count, (Node *)list->head, (Node * )list->tail, (Node * )list->current, list->status);
}