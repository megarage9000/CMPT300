#include "message.h"

void initializeMessageQueues() {
    waitForReceiveQ = List_create();
    waitForReplyQ = List_create();
    messageQ = List_create();
}

void destroyMessageQueues() {
    List_free(waitForReceiveQ, freeProcess);
    List_free(waitForReplyQ, freeProcess);
    List_free(messageQ, freeMessage);
}

void sendMessage(char * message, Process_PCB * process) {

}

void replyMessage(char * message, Process_PCB * process) {

}

void receiveMessage(Process_PCB * process) {

}