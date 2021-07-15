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

// --- Process helper methods --- //

// For tracking which queue a certain process is
// in by storing a List pointer associated with
// the specified Pid.
static List * processTracker[LIST_MAX_NUM_NODES];
static int availablePids[LIST_MAX_NUM_NODES];
static int availablePidIndex = 0;
void initializePidTracking();
int getAvailablePid();
void returnAvailablePid(int pid);
List * getQueueOfProcess(int pid);
void updateProcessTracker(int pid, List * queue);

// A flexible prepend for any queue
int prependToQueue(Process_PCB * process, List * queue);
// A flexible trim for any queue
Process_PCB * trimFromQueue(List * queue);


// --- Process methods --- // 
static List * readyQs[3];
static List * waitForReceiveQ;
static List * waitForReplyQ;

void initializeQueues();

int prependToReadyQueue(Process_PCB * process);
Process_PCB * trimFromReadyQueue(priority processPriority);
int createProcess(priority processPriority);
int forkProcess(Process_PCB * process);
int killProcess(int pid);

bool searchProcess(void * process, void * comparison);

#endif