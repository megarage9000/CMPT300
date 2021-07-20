#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "semaphores.h"

#define MAX_MESSAGE_LENGTH 40
#define INIT_PROCESS_PID -1

// --- Process PCB and Messages --- //
typedef enum priority_e priority;
enum priority_e {low, medium, high, none};
typedef enum state_e state;
enum state_e {blockedReceive, blockedReply, blockedSem, ready, running, stateless};

typedef struct Process_Message_s Process_Message;
struct Process_Message_s {
    int sendingPid;          // Typically the sender / replier
    int receivingPid;        // Typically the receiver       
    char receivedMessage[40];   
};

typedef struct Process_PCB_s  Process_PCB;
struct Process_PCB_s {
    int pid;
    priority processPriority; // low, medium, high
    state processState;       // blockedSend, blockedReply, blockedSem ready, running   
    Process_Message * message;  
};

static Process_Message emptyMessage = {
    .sendingPid = -2,
    .receivingPid = -2,
    .receivedMessage = ""
};  

static Process_PCB emptyProcess = {
    .pid = -2,
    .processPriority = none,
    .processState = stateless,
    .message = NULL
};

Process_PCB initializeProcess(int pid, priority priority, state state);
Process_Message initializeProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize);
void printProcess(Process_PCB process);
void printMessage(Process_Message message);

#endif