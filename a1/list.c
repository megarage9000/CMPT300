#include "list.h"
#include <stdio.h>

List* List_create() {
    if(listInit != LIST_INITIALIZED) {
        // Allocating Lists
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            lists[i] = (List){
                NULL, NULL, NULL
             };
        }
        freeListIndex = LIST_MAX_NUM_HEADS;


        // Allocating Nodes
        lists[LIST_MAX_NUM_HEADS] = (List) {
            NULL, NULL, NULL
        };
        freeNodes = &lists[LIST_MAX_NUM_HEADS];
        Node * prev = NULL;
        for(int i = 0; i < LIST_MAX_NUM_NODES; i++){
            nodes[i] = (Node) {
                NULL, NULL, NULL
            };
            if(prev != NULL) {
                prev->next = &nodes[i];
                nodes[i].prev = prev;
            }

            prev = &nodes[i];
        }

        freeNodes->head = &nodes[0];
        freeNodes->tail = &nodes[LIST_MAX_NUM_NODES];
    }

    List * listToReturn = &lists[freeListIndex--];
    printf("Returned list = %p\n",(List *)listToReturn);
    printf("In Free list stack = %p\n", (List * )&lists[freeListIndex + 1]);
    return listToReturn;
}

int List_count(List* plist) {
    if(plist->head == NULL || plist->tail == NULL){
        return 0;
    }
    Node * node = plist->head;
    int count = 1;
    for(; node->next != NULL; count++) {
        node = node->next;
    }
    return count;
}

