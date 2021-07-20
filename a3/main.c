#include <stdio.h>
#include "commands.h"


void getUserInput(char * buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin);
    buffer[bufferSize] = '\0';
}

void testSemaphores() {
    int id = 0;
    Process_PCB processes[2];
    processes[0] = initializeProcess(0, low, stateless);
    processes[1] = initializeProcess(1, low, stateless);
    initializeSemaphoreArray();
    printf("Creating semaphores\n");
    for(int i = 0; i < NUM_SEMAPHORES; i++) {
        createSemaphore(i, 1);
    }
    printf("Printing semaphores\n");
    printAllSemaphores();
    printf("Processes before V() and P() of semaphore %d:\n", id);
    printProcess(processes[0]);
    printProcess(processes[1]);
    printf("Testing P() on semaphore %d:\n", id);
    for(int i = 0; i < 100; i++) {
        semaphoreP(id, &processes[0]);
    }
    printSemaphore(id);
    printf("Processes after P() of semaphore %d:\n", id);
    printProcess(processes[0]);
    printProcess(processes[1]);
    printf("Testing V() on semaphore %d:\n", id);
    for(int i = 0; i < 2; i++) {
        semaphoreV(id);
    }
    printSemaphore(id);
    printf("Processes after V() and P() of semaphore %d:\n", id);
    printProcess(processes[0]);
    printProcess(processes[1]);
    destroyAllSemaphores();
}

int main() {

}

