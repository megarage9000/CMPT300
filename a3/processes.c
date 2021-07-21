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
        strcpy(processMessage.receivedMessage, message);
    }
    processMessage.state = msg_state;
    return processMessage;
}

void printProcess(Process_PCB process) {
    printf("Process: \n - pid = %d\n - priority = %d\n - state = %d\n", 
        process.pid, process.processPriority, process.processState);
    if(process.message != NULL){
        printMessage(*(process.message));
    }
}

void printMessage(Process_Message message) {
    printf("Process Message: \n - sending pid = %d\n - receiving pid = %d\n - message = %s\n",
        message.sendingPid, message.receivingPid, message.receivedMessage);
}


bool ifEqualProcesses(Process_PCB processA, Process_PCB processB) {
    bool matchingPid = (processA.pid == processB.pid);
    bool matchingPriority = (processA.pid == processB.pid);
    bool matchingState = (processA.processState == processA.processState);
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