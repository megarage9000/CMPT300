#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "stdio.h"
#include "list.h"
#define NUM_SEMAPHORES 5

// --- Semaphores --- //
typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    int s;
    int id;
    List * blockedProccesses;
};

void initialize();
bool semaphoreV(int id);
bool semaphoreP(int id);
void createSemaphore(int id);
Semaphore * getSemaphore(int id);

static Semaphore * semaphores[NUM_SEMAPHORES];

#endif