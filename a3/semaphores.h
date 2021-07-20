#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "stdio.h"
#include "list.h"
#include "processes.h"

#define NUM_SEMAPHORES 5

// --- Semaphores --- //
typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    int s;
    int id;
    List * blockedProccesses;
};

void initializeSemaphoreArray();

Process_PCB * semaphoreV(int id);
void semaphoreP(int id, Process_PCB * process);

void createSemaphore(int id, int sVal);
void destroySemaphore(int id);
Semaphore * getSemaphore(int id);
Process_PCB * dequeueFromSemaphore(int id);
int queueToSemaphore(int id, Process_PCB * process);

void printSemaphore(int id);
void printAllSemaphores();
void destroyAllSemaphores();
static Semaphore * semaphores[NUM_SEMAPHORES];

#endif