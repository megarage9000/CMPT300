#ifndef COMMANDS_H
#define COMMANDS_H

#include "processes.h"
#include "semaphores.h"

#define INIT_PROCESS_PID -1

static List * readyQs[3];
static List * waitForReceiveQ;
static List * waitForReplyQ;
static List * messageQ;
static Process_PCB * currentProcess;
static Process_PCB * initProcess = &(Process_PCB){
    .pid = INIT_PROCESS_PID,
    .processPriority = none,
    .processState = stateless,
    .message = NULL
};

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
// Search for processes
Process_PCB * searchForProcess(int pid);

// --- Process methods --- // 

void initialize();
void initializeQueues();

int prependToReadyQueue(Process_PCB * process);
Process_PCB * trimFromReadyQueue(priority processPriority);


// Simulation methods
int createProcess(priority processPriority);
int forkProcess(Process_PCB * process);
int killProcess(int pid);
void quantum();
void blockProcess(List * queue);

bool searchProcess(void * process, void * comparison);


#endif