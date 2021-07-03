#include "listMonitor.h"
#include "unistd.h"


#include <sys/poll.h>
#include <sys/time.h>

// Testing synchronization on a messageList
int numTimes = 1000;
MessageList * messageList;

bool shutdownProgram;
pthread_mutex_t shutdownLock;

void initializeShutdownLock(){
    shutdownProgram = false;
    pthread_mutex_init(&shutdownLock, NULL);
}

void destroyShutdownLock(){
    pthread_mutex_destroy(&shutdownLock);
}

void setShutdown(bool value){
    pthread_mutex_lock(&shutdownLock);
    //printf("Setting bool shutdown to %d (0 = false, 1 = true)\n", value);
    shutdownProgram = value;
    pthread_mutex_unlock(&shutdownLock);
}

bool getShutdownValue() {
    bool valueToReturn;
    pthread_mutex_lock(&shutdownLock);
    valueToReturn = shutdownProgram;
    pthread_mutex_unlock(&shutdownLock);
    return valueToReturn;
}

void * testConsume(void * id) {
    printf("Starting consumption\n");
    for (int i = 0; i < numTimes; i++) {
        char message[] = "Yo what's poppin";
        consume(messageList, message, strlen(message), (char *)id);
        printf("Successfully consumed message = %s\n", message);
    }
    printf("Finished consumption\n");
    setShutdown(true);
}

void * testProduce(void * id) {
    printf("Starting production\n");
    for(int i = 0; i < numTimes; i++) {
        char receivedMessage[50];
        produce(messageList, receivedMessage, 50, (char *)id);
        printf("Successfully produced message = %s\n", receivedMessage);
    }
    printf("Finished consumption\n");
    setShutdown(true);
}

void * testBlockingIO(void * args) {

    char buf[MAX_MESSAGE_LENGTH];
   
    struct pollfd pfds;
    pfds.fd = STDIN_FILENO;
    pfds.events = POLLIN;

    while(!getShutdownValue()) {
        if(poll(&pfds, 1 , -1)){
            int result = read(STDIN_FILENO, buf, MAX_MESSAGE_LENGTH);
            if(result == 0) {
                fprintf(stderr, "Thread test ERROR: User input has an error!\n");
            }
            else if(result == 1) {
                memset(buf, 0, sizeof buf);
                
            }
            else if(result >= MAX_MESSAGE_LENGTH * sizeof(char)){
                fprintf(stderr, "Threas test ERROR: User input has undefined behaviour!\n");
            }
            else {
                char * message = (char *)malloc(result);
                int sizeInInt = (result / sizeof(char));
                strncpy(message, buf, sizeInInt);
                message[sizeInInt - 1] = '\0';
                printf("User input = %s\n", message);
                // if(strcmp(message, "!") == 0) {
                //     setShutdown(true);
                //     break;
                // }
                free(message);
            }
            memset(buf, 0, sizeof buf);
        }
    }
    
    pthread_exit(NULL);
}
int main() {
    initializeShutdownLock();
    char * ids[4] = {"Message Printer 1", "Message Printer 2", "Message Consumer 1", "Message Consumer 2"};
    pthread_t messagePrinter, messageConsumer, messagePrinter1, messageConsumer1, test;
    messageList = createMessageListPtr();

    pthread_create(&messagePrinter, NULL, testProduce, (void *)ids[0]);
    pthread_create(&messageConsumer, NULL, testConsume,(void *)ids[2]);
    pthread_create(&messagePrinter1, NULL, testProduce,(void *)ids[1]);
    pthread_create(&messageConsumer1, NULL, testConsume,(void *)ids[3]);
    pthread_create(&test, NULL, testBlockingIO, NULL);
    
    pthread_join(messagePrinter, NULL);
    pthread_join(messageConsumer, NULL);
    pthread_join(messagePrinter1, NULL);
    pthread_join(messageConsumer1, NULL);
    pthread_join(test, NULL);

    destroyShutdownLock();
    destroyMessageListPtr(messageList);
    printf("\n ------ \nFinished test! \n ------ \n");
}