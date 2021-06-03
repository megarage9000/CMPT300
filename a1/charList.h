

#ifndef CHAR_LIST
#define CHAR_LIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

static List * charLists[LIST_MAX_NUM_HEADS];

void initialize();

List * createCharList();

int addNewList();

List * getListFromIndex(int index);

void addCharItem(List * charList, char * item);

void insertCharItem(List * charList, char * item);

void appendCharItem(List * charList, char * item);

void prependCharItem(List * charList, char * item);

char * removeCharItem(List * charList);

char * trimList(List * charList);

char * nextItem(List * charList);

char * prevItem(List * charList);

char * getCurrentItem(List * charList);

char * getTailItem(List * charList);

char * getHeadItem(List * charList);

char * trim(List * charList);

void concatLists(List * charList1, int listIndex);

void freeItem(void * item);

void freeList(List * charList, int listIndex);

bool checkMatchingChars(void * item1, void * item2);

char * findItem(List * charList, char * matchingString);

void printList(List * charList);

void printAllLists();

void printNodeInfo(Node * node);

void cleanUp();


#endif