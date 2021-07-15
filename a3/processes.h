#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

#define MAX_MESSAGE_LENGTH 40

// --- Process PCB and Messages --- //

typedef enum priority_e priority;
enum priority_e {low, medium, high, none};
typedef enum state_e state;
enum state_e {blocked, ready, running, stateless};

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
    state processState;       // blocked, ready, running   
    Process_Message message;  //   
};

static Process_Message emptyMessage = {
    .sendingPid = -1,
    .receivingPid = -1,
    .receivedMessage = ""
};  

static Process_PCB emptyProcess = {
    .pid = -1,
    .processPriority = none,
    .processState = stateless,
    .message = (Process_Message){
        -1,
        -1,
        ""
    }
};

Process_PCB initProcess(int pid, priority priority, state state);
Process_Message initProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize);
void printProcess(Process_PCB process);
void printMessage(Process_Message message);

// --- Process methods --- // 
static List * readyQs[3];
static List * waitForSendQ;
static List * waitForReplyQ;

// For tracking the appropriate process, theoretically
// there should be about LIST_MAX_NUM_NODES amount of 
// processes. Available pids are stored in a stack like structure
static priority processTracker[LIST_MAX_NUM_NODES];
static int availablePids[LIST_MAX_NUM_NODES];
static int availablePidIndex = 0;
void initializePidTracking();
int getAvailablePid();
void returnAvailablePid(int pid);

void initializeQueues();
int appendToQueue(Process_PCB * process);
int createProcess(priority processPriority);
int forkProcess(Process_PCB * process);
int killProcess(int pid);

bool searchProcess(void * process, void * comparison);

#endif