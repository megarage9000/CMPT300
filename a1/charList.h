

#ifndef CHAR_LIST
#define CHAR_LIST

#include <stdio.h>
#include "list.h"

static List * charLists[LIST_MAX_NUM_HEADS];

void initialize();

List * createCharList();

List * getListFromIndex(int index);

void addCharItem(List * charList, char * item);

void insertCharItem(List * charList, char * item);

char * removeCharItem(List * charList);

char * nextItem(List * charList);

char * prevItem(List * charList);

char * getCurrentItem(List * charList);

char * getTailItem(List * charList);

char * getHeadItem(List * charList);

void printList(List * charList);

void printNodeInfo(Node * node);

#endif