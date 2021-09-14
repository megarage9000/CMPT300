#ifndef COMMANDS_H
#define COMMANDS_H

#include "processes.h"
#include "semaphores.h"

static List * readyQs[3];
static List * waitForReceiveQ;
static List * waitForReplyQ;
static List * messageQ;


static Process_PCB * currentProcess;
static bool finished = false;

// --- Process methods --- // 
void initializeProgram();
void terminateProgram();

bool isFinished();
Process_PCB * trimFromReadyQueue(priority processPriority);

int getCurrentProcessPID();

// Simulation methods
int createProcess(priority processPriority);

// Forks a process, returns -1 on failure 
int forkProcess();

// Similar to kill, but does this on the current process only
int exitProcess();

// Remove process from the system entirely
// - If the current process happens to be the init_process and
// there are no more process left, terminate
int killProcess(int pid);

// Switches the current process up with ones from readyQs
void quantum();

// Sends a message to a process.
// - Blocks the sending process immediately 
// - Adds the process waiting for a send to given readyQueue, if any
// - Sends the message to messageQ if no process is there to receive
int sendMessage(char * message, int pidToSendTo);

// Replies a message to a process.
// - An unblocking send in simpler terms
int replyMessage(char * message, int pidToReplyTo);

// Procs the process to await a receive
// - Blocks the process if no message is waiting in messageQ
int receiveMessage();

// Prints info on current process
void printProcInfo();

// Prints info of all the system
void totalInfo();


// --- Callers to functions in semaphores.h --- 
int createSem(int id);

int semP(int id);

int semV(int id);

bool ifNoAvailProcesses();
#endif