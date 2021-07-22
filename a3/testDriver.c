#include "commands.h"

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

int main() {
    testSemaphores();
}