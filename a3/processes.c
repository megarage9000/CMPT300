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
Process_Message initializeProcessMessage(int sendingPid, int receivingPid, char * message, int messageSize){

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
    printMessage(*(process.message));
}

void printMessage(Process_Message message) {
    printf("Process Message: \n - sending pid = %d\n - receiving pid = %d\n - message = %s\n",
        message.sendingPid, message.receivingPid, message.receivedMessage);
}

