#include "commands.h"

// TODO: figure out blocking process behaviour
// - Begin testing
// - Implement send / receive / reply
// - Finish implementing Semaphore

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
        List_first(queueToSearch);
        // Find the process
        Process_PCB * process = List_search(queueToSearch, searchProcess, &pid);
        if(process == NULL) {
            return NULL;
        }
        else {
            // Remove the process from the queue and return it
            Process_PCB * process = (Process_PCB*)List_remove(queueToSearch);
            updateProcessTracker(process->pid, NULL);
            returnAvailablePid(process->pid);
            return process;
        }
    }
    else {
        return NULL;
    }
}

// --- Process methods --- // 

void initialize() {
    initializeQueues();
    currentProcess = initProcess;
}

void initializeQueues() {
    readyQs[low] = List_create();
    readyQs[medium] = List_create();
    readyQs[high] = List_create();
}

// Simple abstractions for ready queues
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


// Simulation methods

// Create a process, returns -1 on failure
int createProcess(priority processPriority) {
    Process_PCB * process = (Process_PCB*)malloc(sizeof(Process_PCB));
    *process = initializeProcess(getAvailablePid(), processPriority, ready);
    return prependToReadyQueue(process);
}

// Forks a process, returns -1 on failure 
int forkProcess(Process_PCB * process) {
    Process_PCB * forkedProcess = (Process_PCB *)malloc(sizeof(Process_PCB));
    forkedProcess->processPriority = process->processPriority;
    forkedProcess->message = process->message;
    return prependToReadyQueue(forkedProcess);
}

// Remove process from the system entirely
int killProcess(int pid) {
    Process_PCB * process = searchForProcess(pid);
    if(process != NULL) {
        free(process);
        return 0;
    }
    else{
        return -1;
    }
}

void quantum() {
    // Remove the process and put it back to appropriate queue
     

    // Fetch a new process to run


    // Shift Queues


    // Add new processes if any are unblocked in semaphores and other blocked queues
}

bool searchProcess(void * process, void * comparison) {
    int processPid = ((Process_PCB*)process)->pid;
    return processPid == *((int *)comparison);
}