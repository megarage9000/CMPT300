#ifndef MESSAGE_H
#define MESSAGE_H

#include "processes.h"
#include "list.h"

static List * waitForReceiveQ;
static List * waitForReplyQ;
static List * messageQ;

void initializeMessageQueues();
void destroyMessageQueues();

void sendMessage(char * message, Process_PCB * process);
void replyMessage(char * message, Process_PCB * process);
void receiveMessage(Process_PCB * process);


#endif