#include "list.h"

List* List_create() {

    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        lists[i] = (List){ };
        freeLists[i] = &lists[i];
    }

    freeListIndex = LIST_MAX_NUM_HEADS - 1;

    for(int i = 0; i < LIST_MAX_NUM_NODES; i++ ) {
        nodes[i] = (Node) { };
        freeNodes[i] = &nodes[i];
    }

    freeNodeIndex = LIST_MAX_NUM_NODES - 1;

    List * listToReturn = freeLists[freeNodeIndex--];
    return listToReturn;
}