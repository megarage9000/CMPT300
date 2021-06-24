#include "listMonitor.h"


// Testing synchronization on a messageList
int numTimes = 5000;
MessageList * messageList;

void * testConsume(void * id) {
    for (int i = 0; i < numTimes; i++) {
        char message[] = "Yo what's poppin";
        consume(messageList, message, strlen(message), (char *)id);
        printf("Successfully consumed message = %s\n", message);
    }
}

void * testProduce(void * id) {
    for(int i = 0; i < numTimes; i++) {
        char receivedMessage[50];
        produce(messageList, receivedMessage, (char *)id);
        printf("Successfully produced message = %s\n", receivedMessage);
    }
}
int main() {
    char * ids[4] = {"Message Printer 1", "Message Printer 2", "Message Consumer 1", "Message Consumer 2"};
    pthread_t messagePrinter, messageConsumer, messagePrinter1, messageConsumer1;
    messageList = createMessageListPtr();

    pthread_create(&messagePrinter, NULL, testProduce, (void *)ids[0]);
    pthread_create(&messageConsumer, NULL, testConsume,(void *)ids[2]);
    pthread_create(&messagePrinter1, NULL, testProduce,(void *)ids[1]);
    pthread_create(&messageConsumer1, NULL, testConsume,(void *)ids[3]);
    
    pthread_join(messagePrinter, NULL);
    pthread_join(messageConsumer, NULL);
    pthread_join(messagePrinter1, NULL);
    pthread_join(messageConsumer1, NULL);

    destroyMessageListPtr(messageList);
    printf("\n ------ \nFinished test! \n ------ \n");
}