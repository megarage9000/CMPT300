#include "commands.h"

// TODO: figure out blocking process behaviour
// - Begin testing
// - Implement send / receive / reply
// - Finish implementing Semaphore


// --- Process methods --- // 

void initialize() {
    initializeQueues();
    initializeMessageQueues();
    initializeSemaphoreArray();
    currentProcess = &initProcess;
}

void initializeQueues() {
    readyQs[low] = List_create();
    readyQs[medium] = List_create();
    readyQs[high] = List_create();
}

void initializeMessageQueues() {
    waitForReceiveQ = List_create();
    waitForReplyQ = List_create();
    messageQ = List_create();
}

void destroyQueues() {
    List_free(readyQs[high], freeProcess);
    List_free(readyQs[medium], freeProcess);
    List_free(readyQs[low], freeProcess);
}

void destroyMessageQueues() {
    List_free(waitForReceiveQ, freeProcess);
    List_free(waitForReplyQ, freeProcess);
    List_free(messageQ, freeMessage);
}

void terminateProgram(){
    destroyMessageQueues();
    destroyQueues();
    destroyAllSemaphores();
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        updateProcessTracker(i, NULL);
    }
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
    if(process->pid == INIT_PROCESS_PID){
        return -1;
    }
    Process_PCB * forkedProcess = (Process_PCB *)malloc(sizeof(Process_PCB));
    forkedProcess->processPriority = process->processPriority;
    forkedProcess->message = process->message;
    return prependToReadyQueue(forkedProcess);
}

// Remove process from the system entirely
int killProcess(int pid) {
    if(pid == INIT_PROCESS_PID) {
        terminateProgram();
    }
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
     if(currentProcess != NULL && currentProcess != &initProcess) {
         prependToReadyQueue(currentProcess);
     }

    // Fetch a new process to run
    // - Search from highest to lowest priority
    if(List_count(readyQs[high]) > 0) {
        currentProcess = trimFromReadyQueue(high);
    }
    else if(List_count(readyQs[medium]) > 0) {
        currentProcess = trimFromReadyQueue(medium);
    }
    else if(List_count(readyQs[low]) > 0) {
        currentProcess = trimFromReadyQueue(low);
    }
    else {
        currentProcess = &initProcess;
    }

}




// Sends a message to a process.
// - Blocks the sending process immediately if no process is awaiting
// its send
// - Adds the process waiting for the send to given readyQueue, if any
// - Sends the message to messageQ if no process is there to receive
int sendMessage(char * message, Process_PCB * process, int pidToSendTo) {
    Process_Message * processMessage = (Process_Message *)malloc(sizeof(Process_Message));
    *processMessage = initializeProcessMessage(
        process->pid,
        pidToSendTo,
        message,
        MAX_MESSAGE_LENGTH,
        messageSend
    );

    // If sending to the init process, don't block
    if(pidToSendTo == INIT_PROCESS_PID) {
        initProcess.message = processMessage;
        return 0;
    }

    // Search for a process in waitForRecieveQueue that's waiting for a receive
    Process_PCB* recievingProcess = getProcessFromList(pidToSendTo, waitForReceiveQ);
    // If found, add the process to its associate ready queue
    if(recievingProcess != NULL) {
        recievingProcess->message = processMessage;
        recievingProcess->processState = ready;
        return prependToReadyQueue(recievingProcess);
    }
    // If not, add message to messageQ
    else {
        int resultAddMessage = List_prepend(messageQ, processMessage);

        // If the process is the init process, don't block,
        if(process->pid == INIT_PROCESS_PID) {
            return resultAddMessage;
        }
        else {
            // If the process is not the init process, block and return the results of 
            // adding the queues

            // Check if adding the message resulted an error before blocking process
            if(resultAddMessage == -1) {
                free(processMessage);
                return resultAddMessage;
            }
            else {
                // Add the message to replyQ
                int resultAddToReply = prependToQueue(process, waitForReplyQ);
                // If success, set process to blocked
                if(resultAddToReply != -1){
                    process->processState = blockedSend;
                } 
                return resultAddToReply;
            }
        }
    }
}

// Replies a message to a process.
// - A non-blocking send in simpler terms
// - When it finds a process awaiting for reply, 
// then add to appropriate readyQueue
int replyMessage(char * message, Process_PCB * process, int pidToReplyTo) {
    Process_Message * processMessage = (Process_Message *)malloc(sizeof(Process_Message));
    *processMessage = initializeProcessMessage(
        process->pid,
        pidToReplyTo,
        message,
        MAX_MESSAGE_LENGTH,
        messageReply
    );

    // If sending to the init process, don't block
    if(pidToReplyTo == INIT_PROCESS_PID) {
        initProcess.message = processMessage;
        return 0;
    }

    // Search for a process awaiting a reply in the replyQ
    Process_PCB * processAwaitingReply = getProcessFromList(pidToReplyTo, waitForReplyQ);
    if(process != NULL) {
        processAwaitingReply->message = processMessage;
        processAwaitingReply->processState = ready;
        return prependToReadyQueue(process);
    }

    // If there is no process awaiting the replyQ, free the message.
    // This also handles the initProcess situation, as blocking is not
    // necessary for reply
    free(processMessage);
    return 0;
}

// Procs the process to await a receive
// - Blocks the process if no message is waiting in messageQ
int receiveMessage(Process_PCB * process) {

    // Look for message in messageQ, if any
    Process_Message * receivedMessage = getMessageFromList(process->pid, messageQ, searchMessageReceive);
    if(receivedMessage != NULL) {
        process->message = receivedMessage;    
        return 0;
    }
    // No message to receive
    else {
        // If init, don't block
        if(process->pid == INIT_PROCESS_PID){
            return 0;
        }
        
        // If not init, block
        int result = prependToQueue(process, waitForReceiveQ); 
        if(result != -1){
            process->processState = blockedReceive;
        }  
        return result;
    }

}


