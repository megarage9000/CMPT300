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

// --- Process helper methods --- //
void initializePidTracking(){
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        processTracker[i] = NULL;
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
        availablePids[--availablePidIndex] = pid;
    }
}

List * getQueueOfProcess(int pid){
    if(pid >= 0 && pid < LIST_MAX_NUM_NODES){
        return processTracker[pid];
    }
    return NULL;
}

void updateProcessTracker(int pid, List * queue){
    if(pid >= 0 && pid < LIST_MAX_NUM_NODES) {
        processTracker[pid] = queue;
    }
}

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

// --- Process methods --- // 

void initializeQueues() {
    readyQs[low] = List_create();
    readyQs[medium] = List_create();
    readyQs[high] = List_create();
    waitForReceiveQ = List_create();
    waitForReplyQ = List_create();
}

int prependToReadyQueue(Process_PCB * process) {
    if(process->processPriority != none){
        return prependToQueue(process, readyQs[process->processState]);
    }
    else{
        return -1;
    }
}

Process_PCB * trimFromReadyQueue(priority processPriority){
    if(processPriority != none) {
        return trimFromQueue(readyQs[processPriority]);
    }
    else{
        return NULL;
    }
}

int createProcess(priority processPriority) {
    Process_PCB * process = (Process_PCB*)malloc(sizeof(Process_PCB));
    *process = initProcess(getAvailablePid(), processPriority, ready);
    return prependToReadyQueue(process);
}

int forkProcess(Process_PCB * process) {
    Process_PCB * forkedProcess = (Process_PCB *)malloc(sizeof(Process_PCB));
    forkedProcess->processPriority = process->processPriority;
    forkedProcess->message = process->message;
    return prependToReadyQueue(forkedProcess);
}

int killProcess(int pid) {
    List * queueToSearch = getQueueOfProcess(pid);
    if(queueToSearch != NULL) {
        Process_PCB * process = List_search(queueToSearch, searchProcess, &pid);
        if(process == NULL) {
            return -1;
        }
        else {
            List_remove(queueToSearch);
            updateProcessTracker(process->pid, NULL);
            returnAvailablePid(process->pid);
            return 0;
        }
    }
    else {
        return -1;
    }
}

bool searchProcess(void * process, void * comparison) {
    int processPid = ((Process_PCB*)process)->pid;
    return processPid == *((int *)comparison);
}