
#include "semaphores.h"

void initialize(){
    for(int i = 0; i < NUM_SEMAPHORES; i++){
        semaphores[i] = NULL;
    }
}

bool semaphoreV(int id){
    Semaphore sem = *semaphores[id];
    sem.s++;
    if(sem.s >= 0) return true;
    else return false;
}
bool semaphoreP(int id){
    Semaphore sem = *semaphores[id];
    sem.s--;
    if(sem.s < 0) return true;
    else return false;
}

void createSemaphore(int id) {
    if(semaphores[id] != NULL) {
        semaphores[id] = &(Semaphore) {
            .s = 0,
            .id = id,
            .blockedProccesses = List_create()
        };
    }
}

Semaphore * getSemaphore(int id) {
    return semaphores[id];
}