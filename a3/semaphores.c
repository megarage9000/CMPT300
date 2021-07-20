
#include "semaphores.h"

void initialize(){
    for(int i = 0; i < NUM_SEMAPHORES; i++){
        semaphores[i] = NULL;
    }
}

Process_PCB * semaphoreV(int id){
    Semaphore sem = *semaphores[id];
    sem.s++;
    if(sem.s > 0) {
        return dequeueFromSemaphore(id);
    }
    else{
        return NULL;
    }
}
void semaphoreP(int id, Process_PCB * process){
    Semaphore sem = *semaphores[id];
    if(sem.s > 0) {
        sem.s--;
        if(sem.s <= 0) {
            queueToSemaphore(id, process);
        }
    }
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

Process_PCB * dequeueFromSemaphore(int id) {
    Semaphore * sem = getSemaphore(id);
    return List_trim(sem->blockedProccesses);
}

int queueToSemaphore(int id, Process_PCB * process){
    Semaphore * sem = getSemaphore(id);
    return List_prepend(sem->blockedProccesses, process);
}