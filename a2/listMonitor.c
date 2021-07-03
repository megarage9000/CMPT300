#include "listMonitor.h"

void freeString(void * item){
    free(item);
}

MessageList createMessageList() {
    MessageList messageList;
    memset(&messageList, 0, sizeof messageList);
    messageList.messages = List_create();
    messageList.maxMessages = MAX_NUM_MESSAGES;
    pthread_mutex_init(&messageList.access, NULL);
    pthread_cond_init(&messageList.availableMessage, NULL);
    pthread_cond_init(&messageList.spaceAvailable, NULL);
    return messageList;
}

MessageList * createMessageListPtr(){
    MessageList * messageListPtr = (MessageList *)malloc(sizeof(struct MessageList_s));
    *messageListPtr = createMessageList();
    return messageListPtr;
}

void destroyMessageListPtr(MessageList * messageList){
    List_free(messageList->messages, freeString);
    pthread_mutex_destroy(&messageList->access);
    pthread_cond_destroy(&messageList->availableMessage);
    pthread_cond_destroy(&messageList->spaceAvailable);
    free(messageList);
}

void consume(MessageList * messageList, char * message, int messageLength, char * threadId){
    
    pthread_mutex_lock(&messageList->access);

    // Do a wait if there is no space for the new message
    // - Use a while loop to prevent concurrent consumption
    while(messageList->messages->count >= MAX_NUM_MESSAGES) {
        //printf("Thread %s: No space available at the moment...\n", threadId);
        pthread_cond_wait(&messageList->spaceAvailable, &messageList->access);
    }
    char * newMessage = (char *)malloc(messageLength);
    strncpy(newMessage, message, messageLength);

    // Consume the new message;
    int result = List_prepend(messageList->messages, newMessage);
    if(result == -1) {
        //fprintf(stderr, "Thread %s ERROR: Available nodes have been exhausted\n", threadId);
        //printf("Thread %s: Number of items in messages = %d\n", threadId, messageList->messages->count);
    }
    // Signal process waiting on available messages
    pthread_cond_signal(&messageList->availableMessage);
    pthread_mutex_unlock(&messageList->access);
}

void produce(MessageList * messageList, char * buf, int sizeOfBuf, char * threadId) {

    pthread_mutex_lock(&messageList->access);

     // Do a wait if there is no available message
     // - Use a while to prevent concurrent productions
    while(messageList->messages->count <= 0) {
        //printf("Thread %s: No available messages to produce at the moment...\n", threadId);
        pthread_cond_wait(&messageList->availableMessage, &messageList->access);
    }

    // Get a new message from the messageList
    char * message = List_trim(messageList->messages);
    //printf("Produced new message = %s\n", buf);
    if(message != NULL) {
        strncpy(buf, message, sizeOfBuf);
        free(message);
    }
    else {
        //fprintf(stderr, "Thread %s ERROR: No message available\n", threadId);

    }
    // Signal process waiting for available space to add messages
    pthread_cond_signal(&messageList->spaceAvailable);
    pthread_mutex_unlock(&messageList->access);
}

