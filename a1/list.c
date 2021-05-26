#include "list.h"
#include <stdio.h>

List* List_create() {
    if(listInit != LIST_INITIALIZED) {
        // Allocating Lists
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            lists[i] = (List){
                NULL, NULL, NULL, LIST_OOB_START, 0
             };
        }
        freeListIndex = LIST_MAX_NUM_HEADS - 1;


        // Allocating Nodes
        lists[LIST_MAX_NUM_HEADS] = (List) {
            NULL, NULL, NULL, LIST_NOT_OOB, 0
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
        listInit = LIST_INITIALIZED;
    }

    if(freeListIndex >= 0){
        List * listToReturn = &lists[freeListIndex--];
        return listToReturn;
    }
    else {
        return NULL;
    }
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

int isListOOB(List * pList) {
    return (pList->status == LIST_OOB_END || pList->status == LIST_OOB_START);
}

Node * getFreeNode(void * item) {
    if(freeNodes->tail != NULL){
        // Popping off new node from freeNodes list
        Node * nodeToReturn = freeNodes->tail;
        freeNodes->tail = freeNodes->tail->prev;
        nodeToReturn->prev = NULL;
        nodeToReturn->next = NULL;
        nodeToReturn->item = item;
        return nodeToReturn;
    }
    return NULL;
}

void returnFreeNode(Node * node) {
    // Clearing returning node
    node->item = NULL;
    node->next = NULL;
    node->prev = NULL;

    // Changing tail
    if(freeNodes->tail != NULL) {
        Node * prevTail = freeNodes->tail;
        freeNodes->tail = node;
        prevTail->next = node;
        node->prev = prevTail;
    }
    // Creating new tail when freeNodes is empty
    else{
        freeNodes->tail = node;
    }
}

// Linking and Unlinking Nodes 
typedef enum NodeLinkStatus_e {
    Success,
    Failure
} NodeLinkStatus;

NodeLinkStatus Unlink_node(Node * nodeToRemove, Node * prev, Node * next){
    if(nodeToRemove != NULL){
        if(prev != NULL){
            nodeToRemove->prev = NULL;
            prev->next = NULL;
        }
        if(next != NULL) {
            nodeToRemove->next = NULL;
            next->prev = NULL;
        }
        if(next != NULL && prev != NULL){
            prev->next = next;
            next->prev = prev;
        }
        return Success;
    }
    return Failure;
}

NodeLinkStatus Link_2_nodes(Node * first, Node * second) {
    if(first != second && second != NULL) {
        first->next = second;
        second->prev = first;
        return Success;
    }
    return Failure;
}

NodeLinkStatus Link_3_nodes(Node * first, Node * second, Node * third){
    NodeLinkStatus firstSecond = Link_2_nodes(first, second);
    NodeLinkStatus secondThird = Link_2_nodes(second, third);
    if(firstSecond == secondThird && secondThird == Success){
        return Success;
    }
    else{
        return Failure;
    }
}

void List_add_on_empty(List *pList, Node * newNode){
    pList->head = newNode;
    pList->tail = newNode;
    pList->current = newNode;
}

NodeLinkStatus List_add_start(List * pList, Node * newNode) {
    Node * prevHead = pList->head;
    NodeLinkStatus status = Link_2_nodes(newNode, prevHead);
    if(status == Success){
        pList->head = newNode;
        pList->current = pList->head;
    }
    return status;
}

NodeLinkStatus List_add_end(List * pList, Node * newNode) {
    Node * prevTail = pList->tail;
    NodeLinkStatus status = Link_2_nodes(prevTail, newNode);
    if(status == Success){
        pList->tail = newNode;
        pList->current = pList->tail;
    }
    return status;
}
// ---- List methods ---- //
int List_count(List* pList) {
    if(isListEmpty(pList)){
        return 0;
    }
    else {
        return pList->count;
    }
}

void* List_first(List* pList) {
    if(isListEmpty(pList)){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->head;
    return pList->head->item;
}

void* List_last(List* pList){
    if(isListEmpty(pList)) {
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->tail;
    return pList->tail->item;
}

void* List_next(List* pList){
    if(!isListOOB(pList)){
        pList->current = pList->current->next;
        if(pList->tail->next == pList->current && pList->current == NULL){
            pList->status = LIST_OOB_END;
            return NULL;
        }
        return pList->current->item;
    } 
    return NULL;
}

void* List_prev(List* pList){
    if(!isListOOB(pList)){
        pList->current = pList->current->prev;
        if(pList->head->prev == pList->current && pList->current == NULL){
            pList->status = LIST_OOB_START;
            return NULL;
        }
        return pList->current->item;
    } 
    return NULL;
}

void* List_curr(List* pList){
    return pList->current->item;
}

int List_add(List* pList, void* pItem){
    Node * newNode = getFreeNode(pItem);
    if(newNode != NULL){
        NodeLinkStatus linkingStatus;
        // 1. On Empty list, call add for empty lists
        if(isListEmpty(pList)){
            List_add_on_empty(pList, newNode);
            linkingStatus = Success;
        }
        // 2. If current is pointing to end or OOB end, make the new node the tail
        else if(pList->status == LIST_OOB_END || pList->current == pList->tail) {
            linkingStatus = List_add_end(pList, newNode);
        }
        // 3. If current is pointing to start or OOB start, make the new node the head
        else if(pList->status == LIST_OOB_START || pList->current == pList->head) {
            linkingStatus = List_add_start(pList, newNode);
        }
        // 4. If current is neither head/tail or is the pList empty, do normal add
        else {
            Node * prevCurrent = pList->current;
            linkingStatus = Link_3_nodes(prevCurrent, pList->current, prevCurrent->next);
        }

        if(linkingStatus == Success) {
            pList->status = LIST_NOT_OOB;
            pList->current = newNode;
            pList->count++;
            return 0;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int List_insert(List* pList, void* pItem){
    Node * newNode = getFreeNode(pItem);
    if(newNode != NULL){
        NodeLinkStatus linkingStatus;
        // 1. On Empty list, call add for empty lists
        if(isListEmpty(pList)){
            List_add_on_empty(pList, newNode);
            linkingStatus = Success;
        }
        // 2. If current is pointing to start or OOB start, make the new node the head
        else if(pList->status == LIST_OOB_START || pList->current == pList->head) {
            linkingStatus = List_add_start(pList, newNode);
        }
        // 3. If current is pointing to end or OOB end, make the new node the tail
        else if(pList->status == LIST_OOB_END || pList->current == pList->tail) {
            linkingStatus = List_add_end(pList, newNode);
        }
        // 4. If current is neither head/tail or is the pList empty, do normal insert
        else {
            Node * prevCurrent = pList->current;
            linkingStatus = Link_3_nodes(prevCurrent->prev, newNode, prevCurrent);
            pList->current = newNode;
        }

        if(linkingStatus == Success) {
            pList->status = LIST_NOT_OOB;
            pList->current = newNode;
            pList->count++;
            return 0;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int List_append(List* pList, void* pItem){
    List_last(pList);
    return List_add(pList, pItem);
}

int List_prepend(List* pList, void* pItem){
    List_first(pList);
    return List_insert(pList, pItem);
}

void* List_remove(List* pList){
    if(pList->status == LIST_OOB_END || pList->status == LIST_OOB_START){
        return NULL;
    }
    Node * current = pList->current;
    if(current != NULL) {
        Node * currentPrev = current->prev;
        Node * currentNext = current->next;
        Unlink_node(current, currentPrev, currentNext);
        pList->current = currentNext;
        if(pList->head == current){
            pList->head = currentNext;
        }
        else if(pList->tail == current && pList->current == NULL){
            pList->tail = NULL;
            pList->status = LIST_OOB_END;
        }
        pList->count--;
        return current->item;
    }
    return NULL;
}