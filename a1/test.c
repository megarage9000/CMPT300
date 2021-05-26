#include <stdio.h>
#include "list.h"
#include "charList.h"

void printListInfo(List * list);

int main() {
    initialize();
    printList();
}

void printListInfo(List * list) {
    int count = List_count(list);
    printf("List information:\n - Address: %p\n - Count: %d\n - Head: %p\n - Tail: %p\n - Current: %p\n - Status: %d\n", 
            (List *)list, count, (Node *)list->head, (Node * )list->tail, (Node * )list->current, list->status);
}