#include "list.h"
#include <stdio.h>

List* List_create() {
    if(listInit != LIST_INITIALIZED) {
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            lists[i] = (List){
                NULL, NULL, NULL
             };
            freeLists[i] = &lists[i];
        }

        freeListIndex = LIST_MAX_NUM_HEADS - 1;

        for(int i = 0; i < LIST_MAX_NUM_NODES; i++ ) {
            nodes[i] = (Node) { 
                NULL, NULL, NULL
            };
            freeNodes[i] = &nodes[i];
        }
        freeNodeIndex = LIST_MAX_NUM_NODES - 1;
    }

    List * listToReturn = freeLists[freeListIndex--];
    printf("Returned list = %p\n",(List *)listToReturn);
    printf("In Free list stack = %p\n", (List * )freeLists[freeListIndex + 1]);
    return listToReturn;
}

int List_count(List* plist) {
    if(plist->head == NULL || plist->tail == NULL){
        return 0;
    }
    Node * node = plist->head;
    int count;
    for(count = 0; node->next != NULL; count++) {
        node = node->next;
    }
    return count;
}