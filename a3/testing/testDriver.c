#include "../commands.h"

void testSemaphores() {
    int id = 0;
    initializeProgram();
    createProcess(low);
    createProcess(low);
    quantum();
    printf("Creating semaphores\n");
    for(int i = 0; i < NUM_SEMAPHORES; i++) {
        createSem(i);
    }
    totalInfo();
    printf("Testing P() on semaphore %d:\n", id);
    for(int i = 0; i < 100; i++) {
        semP(id);
    }
    printSemaphore(id);
    printf("Processes after P() of semaphore %d:\n", id);
    totalInfo();
    printf("Testing V() on semaphore %d:\n", id);
    for(int i = 0; i < 2; i++) {
        semV(id);
    }
    printSemaphore(id);
    printf("Processes after V() and P() of semaphore %d:\n", id);
    totalInfo();
    terminateProgram();
}

void testSendReceiveReply() {
    initializeProgram();
    char testMessage[] = "I am clearly longer than 40 characters, let's see how this messsage goes?";
    createProcess(low);
    createProcess(medium);
    createProcess(high);
    quantum();

    totalInfo();

    printf("----- TESTING SEND TO NON-EXISTENT MESSAGES -----\n");
    // Sending to non-existent process
    printProcInfo();
    sendMessage(testMessage, 3);
    printProcInfo();
    sendMessage(testMessage, 3);

    totalInfo();
    printf("----- TESTING RECEIVE FOR PROCESS THAT HAS NO MESSAGES AWAITING IT -----\n");
    // Receiving for a process that has no message for it
    printProcInfo();
    receiveMessage();

    totalInfo();
    printf("----- TESTING RECEIVE FOR PROCESS THAT HAS NO MESSAGES AWAITING IT (INIT)-----\n");
    
    receiveMessage();
    printProcInfo();

    totalInfo();
    printf("----- TESTING UNBLOCKING SENDERS -----\n");
    // Unblocking the senders
    replyMessage(testMessage, 0);
    replyMessage(testMessage, 1);
    totalInfo();

    quantum();
    printf("----- CREATING A NEW PROCESS TO HANDLE SEND TO BLOCKED RECEIVER -----\n");
    // Creating a new process to handle the messages sent to 
    // non-existent process
    createProcess(high);
    receiveMessage();
    receiveMessage();
    totalInfo();

    // send message to process awaiting receive
    sendMessage(testMessage, 2);
    totalInfo();

    quantum();

    totalInfo();
    printf("----- KILLING PROCESSES -----\n");
    // kill all processes
    killProcess(0);
    killProcess(1);
    killProcess(2);
    killProcess(3);


    totalInfo();
    killProcess(INIT_PROCESS_PID);
}

void testAllBlockedProcesses() {
    initializeProgram();

    for(int i = 0; i < 10; i++) {
        createProcess(high);
    }
    quantum();
    totalInfo();

    // have the first 5 block by receive
    for(int i = 0; i < 5; i++){
        receiveMessage();
    }

    totalInfo();

    // have the next 10 block by a semaphore
    createSem(0);
    for(int i = 0; i < 10; i++){
        semP(0);
    }

    totalInfo();

    // at this point, we are expecting the init process to be running, we can
    // call kill()
    killProcess(INIT_PROCESS_PID);
}


int main() {
    testAllBlockedProcesses();
}