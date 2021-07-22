#include "processes.h"

// --- Process PCB and Messages --- //

Process_PCB initializeProcess(int pid, priority priority, state state) {
    return (Process_PCB){
        pid,
        priority,
        state,
        NULL
    };
}
Process_Message initializeProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize, message_state msg_state){
    Process_Message processMessage;
    processMessage.sendingPid = sendingPid;
    processMessage.receivingPid = receivingPid;
    if(messageSize > MAX_MESSAGE_LENGTH){
        return emptyMessage;
    }
    else {
        strncpy(processMessage.receivedMessage, message, MAX_MESSAGE_LENGTH);
    }
    processMessage.state = msg_state;
    return processMessage;
}

// Private functions to convert priority / state to strings

const char * stateToString(state givenState) {
    switch(givenState) {
        case blockedReceive:
            return "blocked on receive";
        case blockedSend:
            return "blocked on send";
        case blockedSem:
            return "blocked on semaphore";
        case ready:
            return "ready to execute";
        case running:
            return "current executing";
        default:
            return "stateless";
    }
}

const char * priorityToString(priority givenPriority){
    switch(givenPriority) {
        case low:
            return "low";
        case medium:
            return "medium";
        case high:
            return "high";
        default:
            return "none";
    }
}

void printProcess(Process_PCB process) {
    printf("Process: \n - pid = %d\n - priority = %s\n - state = %s\n", 
        process.pid, priorityToString(process.processPriority), stateToString(process.processState));
    if(process.message != NULL){
        printMessage(*(process.message));
    }
}

// Private function to convert message state to string
const char * messageStateToString(message_state givenMessageState) {
    switch(givenMessageState) {
        case messageReply:
            return "message reply";
        case messageSend:
            return "message send";
        default:
            return "none";
    }
}

void printMessage(Process_Message message) {
    printf("Process Message: \n - sending pid = %d\n - receiving pid = %d\n - message = %s\n - message state = %s\n",
        message.sendingPid, message.receivingPid, message.receivedMessage, messageStateToString(message.state));
}


bool ifEqualProcesses(Process_PCB processA, Process_PCB processB) {
    bool matchingPid = (processA.pid == processB.pid);
    bool matchingPriority = (processA.pid == processB.pid);
    bool matchingState = (processA.processState == processB.processState);
    bool matchingMessage = (processA.message == processB.message);
    return (matchingPid && matchingPriority && matchingState && matchingMessage);
}


void freeProcess(void * process) {
    void* message = ((Process_PCB*)process)->message;
    freeMessage(message);
    free(process);
    process = NULL;
}

void freeMessage(void * message){
    free(message);
    message = NULL;
}

// --- For List_Search ---- //

// Process_PCB
bool searchProcess(void * process, void * comparison) {
    int processPid = ((Process_PCB*)process)->pid;
    return processPid == *((int *)comparison);
}

Process_PCB * getProcessFromList(int pid, List * list) {
    List_first(list);
    Process_PCB * process = List_search(list, ((void*)&pid), searchProcess);
    if(process != NULL) {
        return List_remove(list);
    }
    return process;
}

// Process_Message
// - For process messages awaiting receives / replies
bool searchMessageReceive(void * message, void * receivingPid) {
    Process_Message * messagePtr = ((Process_Message *)message);
    int receive = *((int *)receivingPid);
    return messagePtr->receivingPid == receive;
}

bool searchMessageSending(void * message, void * sendingPid){
    Process_Message * messagePtr = ((Process_Message *)message);
    int send = *((int *)sendingPid);
    return messagePtr->sendingPid == send;
}

Process_Message * getMessageFromList(int pid, List * list, COMPARATOR_FN compareFunc){
    List_first(list);
    Process_Message * message = List_search(list, ((void*)&pid), compareFunc);
    if(message != NULL) {
        return List_remove(list);
    }
    return message;
}

// --- Process helper methods --- //
void initializePidTracking(){
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        processTracker[i] = NULL;
        availablePids[i] = i;
    }
}
// Gets available Pid for a new process
int getAvailablePid(){
    if(availablePidIndex < LIST_MAX_NUM_NODES){
        return availablePids[availablePidIndex++];
    }
    else{
        return -1;
    }
}

// Recycles Pid for new processes
void returnAvailablePid(int pid){
    if(availablePidIndex > 0) {
        availablePids[--availablePidIndex] = pid;
    }
}

// Returns the associated queue of a process with 
// given pid
List * getQueueOfProcess(int pid){
    if(pid >= 0 && pid < LIST_MAX_NUM_NODES){
        return processTracker[pid];
    }
    return NULL;
}

// Updates the associated queue of process
void updateProcessTracker(int pid, List * queue){
    if(pid >= 0 && pid < LIST_MAX_NUM_NODES) {
        processTracker[pid] = queue;
    }
}

// Adds process to a queue, also executes updateProcessTracker
// for associate queue
int prependToQueue(Process_PCB * process, List * queue) {
    if(List_prepend(queue, process) == -1){
        free(process);
        return -1;
    }
    else {
        updateProcessTracker(process->pid, queue);
        return 0;
    }
}

// Removes process from a queue, also updates the associated
// queue to NULL
Process_PCB * trimFromQueue(List * queue) {
    Process_PCB * process = (Process_PCB *)List_trim(queue);
    if(process != NULL) {
        updateProcessTracker(process->pid, NULL);
        return process;
    }
    else {
        return NULL;
    }

}

// Finds the process of given pid and removes it, also
// updates process tracker to NULL
Process_PCB * searchForProcess(int pid){
    List * queueToSearch = getQueueOfProcess(pid);
    if(queueToSearch != NULL) {
        Process_PCB * process = getProcessFromList(pid, queueToSearch);
        if(process != NULL) {
            updateProcessTracker(pid, NULL);
            return NULL;
        }
        else{
            return NULL;
        }
    }
    else {
        return NULL;
    }
}
