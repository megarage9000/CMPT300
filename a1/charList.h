

#ifndef CHAR_LIST
#define CHAR_LIST

#include <stdio.h>
#include "list.h"

static List * charList = NULL;

void initialize();

void addCharItem(char * item);

void insertCharItem(char * item);

char * removeCharItem();

char * nextItem();

char * prevItem();

char * getCurrentItem();

char * getTailItem();

char * getHeadItem();

void printList();

#endif