#include "listMonitor.h"


// Testing synchronization on a messageList
int numTimes = 2000;
MessageList * messageList;

void testConsume() {
    for (int i = 0; i < numTimes; i++) {
        char message[] = "Yo what's poppin";
        consume(messageList, message);
        printf("Successfully consumed message = %s\n", message);
    }
}

void testProduce() {
    for(int i = 0; i < numTimes; i++) {
        char receivedMessage[50];
        produce(messageList, receivedMessage);
        printf("Successfully produced message = %s\n", receivedMessage);
    }
}
int main() {
    pthread_t messagePrinter, messageConsumer;
    messageList = createMessageListPtr();

    pthread_create(&messagePrinter, NULL, testProduce, NULL);
    pthread_create(&messageConsumer, NULL, testConsume, NULL);
    
    pthread_join(messagePrinter, NULL);
    pthread_join(messageConsumer, NULL);

    destroyMessageListPtr(messageList);
    printf("\n ------ \nFinished test! \n ------ \n");
}