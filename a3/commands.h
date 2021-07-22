#ifndef COMMANDS_H
#define COMMANDS_H

#include "processes.h"
#include "semaphores.h"

static List * readyQs[3];
static List * waitForReceiveQ;
static List * waitForReplyQ;
static List * messageQ;


static Process_PCB * currentProcess;


// --- Process methods --- // 
void initializeProgram();
void terminateProgram();

int prependToReadyQueue(Process_PCB * process);
Process_PCB * trimFromReadyQueue(priority processPriority);


// Simulation methods
int createProcess(priority processPriority);
int forkProcess();
int killProcess(int pid);
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

#endif