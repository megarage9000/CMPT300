#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

#define MAX_MESSAGE_LENGTH 40
#define INIT_PROCESS_PID -56

#define FAILURE -1
#define SUCCESS 0

const char * actionResultToString(int result);

// --- Process PCB and Messages --- //

// Priority values
typedef enum priority_e priority;
enum priority_e {high, medium, low, none};
const char * priorityToString(priority givenPriority);

// State values
typedef enum state_e state;
enum state_e {blockedReceive, blockedSend, blockedSem, ready, running, stateless};

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

static Process_PCB initProcess = (Process_PCB){
    .pid = INIT_PROCESS_PID,
    .processPriority = none,
    .processState = ready,
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

// For List_Search
bool searchProcess(void * process, void * comparison);
// Looks for a specific process in a list. If found, remove item
// and return, else return NULL
Process_PCB * getProcessFromList(int pid, List * list);

// For List_Search (Messages)
// - For process messages awaiting receives / replies
bool searchMessageReceive(void * message, void * receivingPid);
// - For process messages that have the associated sending pid
bool searchMessageSending(void * message, void * sendingPid);
// - Get a message with a given message COMPARATOR_FN
Process_Message * getMessageFromList(int pid, List * list, COMPARATOR_FN compare);

// For tracking which queue a certain process is
// in by storing a List pointer associated with
// the specified Pid.
static List * processTracker[LIST_MAX_NUM_NODES];
static int availablePids[LIST_MAX_NUM_NODES];
static int availablePidIndex = 0;

// - Pid management
void initializePidTracking();
// Gets available Pid for a new process
int getAvailablePid();
// Recycles Pid for new processes
void returnAvailablePid(int pid);

// Getting associated queues and updating queues
List * getQueueOfProcess(int pid);
void updateProcessTracker(int pid, List * queue);

// Retrieving / Adding processes to queues

// Adds process to a queue, also executes updateProcessTracker
// for associate queue
int prependToQueue(Process_PCB * process, List * queue);

// Removes process from a queue, also updates the associated
// queue to NULL
Process_PCB * trimFromQueue(List * queue);

// Finds the process of given pid and removes it, also
// updates process tracker to NULL
Process_PCB * searchForProcess(int pid);

// Checking if no processes exist
bool ifNoMoreProcess();

// Checking if process is blocked
bool isProcessBlocked(Process_PCB process);
#endif