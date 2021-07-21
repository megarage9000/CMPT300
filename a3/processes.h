#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

#define MAX_MESSAGE_LENGTH 40


// --- Process PCB and Messages --- //

// Priority values
typedef enum priority_e priority;
enum priority_e {low, medium, high, none};

// State values
typedef enum state_e state;
enum state_e {blockedReceive, blockedReply, blockedSem, ready, running, stateless};

// Message state values
typedef enum message_state_e message_state;
enum message_state_e {messageSend, messageReply, messageNone};

// The Process Message Construct
typedef struct Process_Message_s Process_Message;
struct Process_Message_s {
    int sendingPid;          // Typically the sender / replier
    int receivingPid;        // Typically the receiver       
    char receivedMessage[40];  
    message_state state; 
};

// Process Control Block Construct
typedef struct Process_PCB_s  Process_PCB;
struct Process_PCB_s {
    int pid;
    priority processPriority; // low, medium, high
    state processState;       // blockedSend, blockedReply, blockedSem ready, running   
    Process_Message * message;  
};

// Empty Message and Process, may delete in a future iteration
static const Process_Message emptyMessage = {
    .sendingPid = -2,
    .receivingPid = -2,
    .receivedMessage = "",
    .state = messageNone
};  

static const Process_PCB emptyProcess = {
    .pid = -2,
    .processPriority = none,
    .processState = stateless,
    .message = NULL
};

// Creating Processes / Messages
Process_PCB initializeProcess(int pid, priority priority, state state);
Process_Message initializeProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize,  message_state msg_state);

// Printing Process and Message information
// - Consider using Ncurses?
void printProcess(Process_PCB process);
void printMessage(Process_Message message);

// Comparing processes in the struct level
bool ifEqualProcesses(Process_PCB processA, Process_PCB processB);
void freeProcess(void * process);
void freeMessage(void * message);

#endif