#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>

// --- PCB --- //

enum priority {low, medium, high, none};
enum state {blocked, ready, running, stateless};

typedef struct Process_Message_s Process_Message;
struct Process_Message_s {
    int sendingPid;          // Typically the sender / replier
    int receivingPid;        // Typically the receiver       
    char receivedMessage[40];   
};

typedef struct Process_PCB_s  Process_PCB;
struct Process_PCB_s {
    int pid;
    priority processPriority; // low, medium, high
    state processState;       // blocked, ready, running   
    Process_Message message;  //   
};

constexpr Process_Message emptyMessage = {
    -1,
    -1,
    ""
};  

constexpr Process_PCB emptyProcess = {
    -1,
    none,
    stateless,
    emptyMessage
};

Process_PCB createProcess(int pid, priority priority, state state);
Process_Message createMessage(int sendingPid, int receivingPid, char * message);

#endif