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

// Storage for the semaphores. Since there
// are a fixed amount, we use a static array
static Semaphore * semaphores[NUM_SEMAPHORES];
void initializeSemaphoreArray();

// Blocking / Unblocking methods
Process_PCB * semaphoreV(int id);
void semaphoreP(int id, Process_PCB * process);

// Creating / Destroying semaphores
void createSemaphore(int id, int sVal);
void destroySemaphore(int id);
void destroyAllSemaphores();

// Accessing / Dequeue / Queue semaphores
Semaphore * getSemaphore(int id);
Process_PCB * dequeueFromSemaphore(int id);
int queueToSemaphore(int id, Process_PCB * process);

// Printing Semaphores for debugging
// - Consider using Ncurses?
void printSemaphore(int id);
void printAllSemaphores();


#endif