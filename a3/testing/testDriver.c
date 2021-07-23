#include "../commands.h"

void testSemaphores() {
    int id = 0;
    initializeProgram();
    printf("%s\n", actionResultToString(createProcess(low)));
    printf("%s\n", actionResultToString(createProcess(low)));

    quantum();
    for(int i = 0; i < NUM_SEMAPHORES; i++) {
        printf("%s\n", actionResultToString(createSem(i)));
    }
    totalInfo();
    for(int i = 0; i < 100; i++) {
        printf("%s\n", actionResultToString(semP(id)));
    }
    totalInfo();
    for(int i = 0; i < 2; i++) {
        printf("%s\n", actionResultToString(semV(id)));
    }
    quantum();
    totalInfo();
    printf("%s\n", actionResultToString(exitProcess()));
    totalInfo();
    printf("%s\n", actionResultToString(exitProcess()));
    totalInfo();
    printf("%s\n", actionResultToString(exitProcess()));
}

void testSendReceiveReply() {
    initializeProgram();
    char testMessage[] = "I am clearly longer than 40 characters, let's see how this messsage goes?";
    printf("%s\n", actionResultToString(createProcess(low)));
    printf("%s\n", actionResultToString(createProcess(medium)));
    printf("%s\n", actionResultToString(createProcess(high)));
    quantum();

    totalInfo();

    printf("----- TESTING SEND TO NON-EXISTENT MESSAGES -----\n");
    // Sending to non-existent process
    printProcInfo();
    printf("%s\n", actionResultToString(sendMessage(testMessage, 3)));
    printProcInfo();
    printf("%s\n", actionResultToString(sendMessage(testMessage, 3)));

    totalInfo();
    printf("----- TESTING RECEIVE FOR PROCESS THAT HAS NO MESSAGES AWAITING IT -----\n");
    // Receiving for a process that has no message for it
    printProcInfo();
    printf("%s\n", actionResultToString(receiveMessage()));

    totalInfo();
    printf("----- TESTING RECEIVE FOR PROCESS THAT HAS NO MESSAGES AWAITING IT (INIT)-----\n");
    
    printf("%s\n", actionResultToString(receiveMessage()));
    printProcInfo();

    totalInfo();
    printf("----- TESTING UNBLOCKING SENDERS -----\n");
    // Unblocking the senders
    printf("%s\n", actionResultToString(replyMessage(testMessage, 0)));
    printf("%s\n", actionResultToString(replyMessage(testMessage, 1)));
    totalInfo();

    quantum();
    printf("----- CREATING A NEW PROCESS TO HANDLE SEND TO BLOCKED RECEIVER -----\n");
    // Creating a new process to handle the messages sent to 
    // non-existent process
    printf("%s\n", actionResultToString(createProcess(high)));
    printf("%s\n", actionResultToString(receiveMessage()));
    printf("%s\n", actionResultToString(receiveMessage()));
    totalInfo();

    // send message to process awaiting receive
    printf("%s\n", actionResultToString(sendMessage(testMessage, 2)));
    totalInfo();

    quantum();

    totalInfo();
    printf("----- KILLING PROCESSES -----\n");
    // kill all processes
    printf("%s\n", actionResultToString(killProcess(0)));
    printf("%s\n", actionResultToString(killProcess(1)));
    printf("%s\n", actionResultToString(killProcess(2)));
    printf("%s\n", actionResultToString(killProcess(3)));

    totalInfo();
    printf("%s\n", actionResultToString(killProcess(INIT_PROCESS_PID)));
}

void testSendReceiveSimple(){
    initializeProgram();

    char * message = "RANDOM HELLO";

    createProcess(low);
    createProcess(low);
    createProcess(low);
    createProcess(high);
    quantum();

    totalInfo();

    printf("%s\n", actionResultToString(sendMessage(message, 0)));
    printf("%s\n", actionResultToString(sendMessage(message, 1)));
    printf("%s\n", actionResultToString(sendMessage(message, 10)));
    printf("%s\n", actionResultToString(sendMessage(message, 131)));
    printf("%s\n", actionResultToString(sendMessage(message, 31)));
    
    totalInfo();

    printf("%s\n", actionResultToString(replyMessage(message, 0)));
    printf("%s\n", actionResultToString(replyMessage(message, 1)));

    totalInfo();

    quantum();

    printf("%s\n", actionResultToString(exitProcess()));
    printf("%s\n", actionResultToString(exitProcess()));

    totalInfo();

    printf("%s\n", actionResultToString(killProcess(2)));
    printf("%s\n", actionResultToString(killProcess(3)));
    //killProcess(-1);

    totalInfo();

    printf("%s\n", actionResultToString(exitProcess()));

    totalInfo();
}

void testExhaustion(){
    initializeProgram();

    // Should work
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        createProcess(high);
    }

    totalInfo(); 

    // Should not work
    for(int i = 0;  i < 10; i++) {
        sendMessage("Should not work", -123);
    }

    totalInfo();

    // Should work
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++){
        killProcess(i);
    }

    totalInfo();

    // Should work
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++){
        sendMessage("SHould work", -12);
    }

    totalInfo();

    exitProcess();


}
int main() {
    //testSendReceiveSimple();
    //testSemaphores();
    //testSendReceiveReply();
    testExhaustion();
}