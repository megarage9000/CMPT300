#ifndef PROCCESSES_H
#define PROCCESSES_H

#include <stdio.h>
#include <stdlib.h>

// --- PCB --- //

enum priority {low, medium, high};
enum state {blocked, ready, running};

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

Process_PCB createProcess(int pid, priority priority, state state);
Process_Message createMessage(int sendingPid, int receivingPid, char * message);

#endif