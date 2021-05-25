#include "list.h"
#include <stdio.h>

List* List_create() {
    if(listInit != LIST_INITIALIZED) {
        // Allocating Lists
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            lists[i] = (List){
                NULL, NULL, NULL, LIST_NOT_OOB
             };
        }
        freeListIndex = LIST_MAX_NUM_HEADS - 1;


        // Allocating Nodes
        lists[LIST_MAX_NUM_HEADS] = (List) {
            NULL, NULL, NULL, LIST_NOT_OOB
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
    return listToReturn;
}

// ---- Private methods ---- //
int isListEmpty(List * pList) {
    if(pList->head == NULL && pList->tail == NULL && pList->current == NULL){
        return 1;
    }
    else {
        return 0;
    }
}

Node * getFreeNode() {
    if(freeNodes->tail != NULL){
        Node * nodeToReturn = freeNodes->tail;
        freeNodes->tail = freeNodes->tail->prev;
        nodeToReturn->prev = NULL;
        nodeToReturn->next = NULL;
        return nodeToReturn;
    }
    return NULL;
}

void returnFreeNode(Node * node) {
    node->item = NULL;
    node->next = NULL;
    node->prev = NULL;
    if(freeNodes->tail != NULL) {
        Node * prevTail = freeNodes->tail;
        freeNodes->tail = node;
        prevTail->next = node;
        node->prev = prevTail;
    }
    else{
        freeNodes->tail = node;
    }
}

// ---- List methods ---- //
int List_count(List* pList) {
    if(pList->head == NULL || pList->tail == NULL){
        return 0;
    }
    Node * node = pList->head;
    int count = 1;
    for(; node->next != NULL; count++) {
        node = node->next;
    }
    return count;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
    if(isListEmpty(pList)){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->head;
    return pList->head->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    if(isListEmpty(pList)) {
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->tail;
    return pList->tail->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    if(pList->current != NULL){
        pList->current = pList->current->next;
        if(pList->tail->next == pList->current){
            pList->status = LIST_OOB_END;
            return NULL;
        }
        return pList->current->item;
    } 
    return NULL;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    if(pList->current != NULL){
        pList->current = pList->current->prev;
        if(pList->head->prev == pList->current){
            pList->status = LIST_OOB_START;
            return NULL;
        }
        return pList->current->item;
    } 
    return NULL;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    return pList->current->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){
    if(pList->status == LIST_OOB_START) {
        
    } 
    else if(pList->status == LIST_OOB_END) {

    }
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem);

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem);

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem);

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList);