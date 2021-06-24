#ifndef LIST_MONITOR
#define LIST_MONITOR
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"

#define MAX_NUM_MESSAGES 10
#define MAX_MESSAGE_LENGTH 1000

typedef struct messageList_s MessageList;
struct messageList_s {
    List * messages;
    int maxMessages;
    pthread_mutex_t access;
    pthread_cond_t availableMessage;
    pthread_cond_t spaceAvailable;
};

void freeString(void * item);

MessageList createMessageList();

void destroyMessageListPtr(MessageList * messageList);

void consume(MessageList * messageList, char * message);

void produce(MessageList * messageList, char * buf);

#endif