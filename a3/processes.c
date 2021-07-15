#include "processes.h"

// --- Process PCB and Messages --- //

Process_PCB initProcess(int pid, priority priority, state state) {
    return (Process_PCB){
        pid,
        priority,
        state,
        emptyMessage
    };
}
Process_Message initProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize){

    Process_Message processMessage;
    processMessage.sendingPid = sendingPid;
    processMessage.receivingPid = receivingPid;
    if(messageSize > MAX_MESSAGE_LENGTH){
        return emptyMessage;
    }
    else {
        strcpy(processMessage.receivedMessage, message);
    }
    return processMessage;
}

void printProcess(Process_PCB process) {
    printf("Process: \n - pid = %d\n - priority = %d\n - state = %d\n", 
        process.pid, process.processPriority, process.processState);
    printMessage(process.message);
}

void printMessage(Process_Message message) {
    printf("Process Message: \n - sending pid = %d\n - receiving pid = %d\n - message = %s\n",
        message.sendingPid, message.receivingPid, message.receivedMessage);
}

// --- Process methods --- // 

void initializePidTracking(){
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        processTracker[i] = none;
        availablePids[i] = i;
    }
}
int getAvailablePid(){
    if(availablePidIndex < LIST_MAX_NUM_NODES){
        return availablePids[availablePidIndex++];
    }
    else{
        return -1;
    }
}
void returnAvailablePid(int pid){
    if(availablePidIndex > 0) {
        availablePids[availablePidIndex--] = pid;
    }
}

void initializeQueues() {
    readyQs[low] = List_create();
    readyQs[medium] = List_create();
    readyQs[high] = List_create();
    waitForSendQ = List_create();
    waitForReplyQ = List_create();
}

int appendToQueue(Process_PCB * process) {
    if(List_append(readyQs[process->processPriority], process) == -1){
        free(process);
        return -1;
    }
    else {
        return 0;
    }
}

int createProcess(priority processPriority) {
    Process_PCB * process = (Process_PCB*)malloc(sizeof(Process_PCB));
    *process = initProcess(getAvailablePid(), processPriority, ready);
    return appendToQueue(process);
}

int forkProcess(Process_PCB * process) {
    Process_PCB * forkedProcess = (Process_PCB *)malloc(sizeof(Process_PCB));
    forkedProcess->processPriority = process->processPriority;
    forkedProcess->message = process->message;
    return appendToQueue(forkedProcess);
}

int killProcess(int pid) {
    priority processPriority = processTracker[pid];
    Process_PCB * process = List_search(readyQs[processPriority], searchProcess, &pid);
    if(process == NULL) {
        return -1;
    }
    else {
        List_remove(readyQs[processPriority]);
        return 0;
    }
}

bool searchProcess(void * process, void * comparison) {
    int processPid = ((Process_PCB*)process)->pid;
    return processPid == *((int *)comparison);
}