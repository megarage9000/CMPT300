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

void destroyMessageListPtr(MessageList * messageList){
    List_free(messageList->messages, freeString);
    pthread_mutex_destroy(&messageList->access);
    pthread_cond_destroy(&messageList->availableMessage);
    pthread_cond_destroy(&messageList->spaceAvailable);
}

void consume(MessageList * messageList, char * message){
    printf("Starting consumption...\n");
    
    pthread_mutex_lock(&messageList->access);

    // Do a wait if there is no space for the new message
    if(messageList->messages->count == MAX_NUM_MESSAGES) {
        printf("No space available at the moment...\n");
        pthread_cond_wait(&messageList->spaceAvailable, &messageList->access);
    }

    // Consume the new message;
    char * newMessage = (char *)malloc(MAX_MESSAGE_LENGTH);
    strcpy(newMessage, message);
    List_prepend(messageList->messages, newMessage);
    printf("Added message = %s\n", newMessage);

    // Signal process waiting on available messages
    pthread_cond_signal(&messageList->availableMessage);

    pthread_mutex_unlock(&messageList->access);

}

void produce(MessageList * messageList, char * buf) {
    printf("Starting production...\n");

    pthread_mutex_lock(&messageList->access);

     // Do a wait if there is no available message
    if(messageList->messages->count == 0) {
        printf("No available messages to produce at the moment...\n");
        pthread_cond_wait(&messageList->availableMessage, &messageList->access);
    }

    // Get a new message from the messageList
    char * message = List_trim(messageList->messages);
    strcpy(buf, message);
    free(message);
    printf("Produced new message = %s\n", buf);

    // Signal process waiting for available space to add messages
    pthread_cond_signal(&messageList->spaceAvailable);

    pthread_mutex_unlock(&messageList->access);
}