#ifndef LIST_MONITOR
#define LIST_MONITOR
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"

#define MAX_NUM_MESSAGES 10
#define MAX_MESSAGE_LENGTH 1000

typedef struct MessageList_s MessageList;
struct MessageList_s {
    List * messages;
    int maxMessages;

    // For one process access
    pthread_mutex_t access;

    // To notify blocked producers
    pthread_cond_t availableMessage;

    // To notify blocked consumers
    pthread_cond_t spaceAvailable;
};

void freeString(void * item);

MessageList createMessageList();

MessageList * createMessageListPtr();

void destroyMessageListPtr(MessageList * messageList);

void consume(MessageList * messageList, char * message, char * threadId);

void produce(MessageList * messageList, char * buf, char * threadId);

#endif