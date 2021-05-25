#include <stdio.h>
#include "list.h"

int main() {
    List * list = List_create();
    int count = List_count(list);

    printf("List information:\n - Address: %p\n - Count: %d\n - Head: %p\n - Tail: %p\n - Current: %p\n", 
            (List *)list, count, (Node *)list->head, (Node * )list->tail, (Node * )list->current);
}