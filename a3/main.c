#include <stdio.h>
#include "commands.h"



char * CREATE = "C";
char * FORK = "F";
char * KILL = "K";
char * EXIT = "E";
char * QUANTUM = "Q";
char * SEND = "S";
char * RECEIVE = "R";
char * REPLY = "Y";
char * NEW_SEM = "N";
char * SEM_P = "P";
char * SEM_V = "V";
char * PROC_I = "I";
char * TOTAL_I = "T";

void getUserInput(char * buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

bool compareString(char * stringA, char * stringB) {
    return(strcmp(stringA, stringB) == 0);
}

int getStringToInt(char * stringInput) {
    return strtol(stringInput, NULL, 10);
}

bool executeCommandLoop(char * userInput){

    printf("Entered command: %s\n", userInput);
    // Create
    if(compareString(userInput, CREATE)){
        char newInput[MAX_MESSAGE_LENGTH];
        printf("Enter priority of new process: \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int priority = getStringToInt(newInput);
        printf("%s\n", actionResultToString(createProcess(priority)));
    }

    // Fork
    else if (compareString(userInput , FORK)){
        printf("%s\n", actionResultToString(forkProcess()));
    }

    // Kill
    else if (compareString(userInput , KILL)){
        char newInput[MAX_MESSAGE_LENGTH];;
        printf("Enter PID of process to kill: \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int pid = getStringToInt(newInput);
        int result = killProcess(pid);
        printf("%s\n", actionResultToString(result));
        return isFinished();
    }

    // Exit
    else if (compareString(userInput , EXIT)){
        int result = exitProcess();
        printf("%s\n", actionResultToString(result));
        return isFinished();
    }

    // Quantum
    else if (compareString(userInput , QUANTUM)){
        quantum();
    }

    // Send
    else if (compareString(userInput , SEND)){
        char newInput[MAX_MESSAGE_LENGTH];

        printf("Enter PID of process to send to: \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int pid = getStringToInt(newInput);
        printf("Enter message to send (Will be truncated to 40 characters): \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        printf("%s\n", actionResultToString(sendMessage(newInput, pid)));
    }

    // Receive
    else if (compareString(userInput , RECEIVE)){
        printf("%s\n", actionResultToString(receiveMessage()));
    }

    else if(compareString(userInput, REPLY)) {
        char newInput[MAX_MESSAGE_LENGTH];
        printf("Enter PID of process to reply to: \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int pid = getStringToInt(newInput);
        printf("Enter message to reply (Will be truncated to 40 characters): \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        printf("%s\n", actionResultToString(replyMessage(newInput, pid)));
    }

    // New Semaphore
    else if (compareString(userInput , NEW_SEM)){
        char newInput[MAX_MESSAGE_LENGTH];
        printf("Enter ID of new semaphore: \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int id = getStringToInt(newInput);
        printf("%s\n", actionResultToString(createSem(id)));
    }

    // Semaphore P
    else if (compareString(userInput , SEM_P)){
        char newInput[MAX_MESSAGE_LENGTH];
        printf("Enter ID of semaphore to perform P(): \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int id = getStringToInt(newInput);
        printf("%s\n", actionResultToString(semP(id)));
    }

    // Semaphore V
    else if (compareString(userInput , SEM_V)){
        char newInput[MAX_MESSAGE_LENGTH];
        printf("Enter ID of semaphore to perform V(): \n");
        getUserInput(newInput, MAX_MESSAGE_LENGTH);
        int id = getStringToInt(newInput);
        printf("%s\n", actionResultToString(semV(id)));
    }

    // Process Info
    else if (compareString(userInput , PROC_I)){
        printProcInfo();
    }

    // Total System Info
    else if (compareString(userInput , TOTAL_I)){
        totalInfo();
    }

    return false;
}

int main() {
    initializeProgram();
    while(true){
        char input[MAX_MESSAGE_LENGTH];
        printf("Command: \n");
        getUserInput(input, MAX_MESSAGE_LENGTH);
        if(executeCommandLoop(input)){
            break;
        }
    }
}

