
#include "semaphores.h"

void initializeSemaphoreArray(){
    for(int i = 0; i < NUM_SEMAPHORES; i++){
        semaphores[i] = NULL;
    }
}

Process_PCB * semaphoreV(int id){
    Semaphore * sem = semaphores[id];
    if(sem != NULL){
        sem->s++;
        if(sem->s > 0) {
            Process_PCB * process = dequeueFromSemaphore(id);
            if(process != NULL) {
                printf("Unblocking process from semaphore %d!\n", id);
                process->processState = ready;
                return process;
            } 
        }
    }
    return NULL;
}

void semaphoreP(int id, Process_PCB * process){
    if(process == NULL){
        return;
    }
    Semaphore * sem = semaphores[id];
    if(sem != NULL) {
        if(sem->s > 0) {
            sem->s--;
            if(sem->s <= 0) {
                printf("Blocking process with id %d!\n", process->pid);
                process->processState = blockedSem;
                queueToSemaphore(id, process);
            }
        }
    }
}

void createSemaphore(int id, int sVal) {
    if(semaphores[id] == NULL) {
        semaphores[id] = (Semaphore *)malloc(sizeof(Semaphore));
        semaphores[id]->id = id;
        if(sVal < 0 ) sVal = 0;
        semaphores[id]->s = sVal;
        semaphores[id]->blockedProccesses = List_create();
    }
}

void destroySemaphore(int id) {
    Semaphore * sem = getSemaphore(id);
    if(sem != NULL) {
        List_free(sem->blockedProccesses, freeProcess);
        free(sem);
        sem = NULL;
    }
}

Semaphore * getSemaphore(int id) {
    if(id >= 0 && id < NUM_SEMAPHORES){
        return semaphores[id];
    }
    else{
        return NULL;
    }
}

Process_PCB * dequeueFromSemaphore(int id) {
    Semaphore * sem = getSemaphore(id);
    return List_trim(sem->blockedProccesses);
}

int queueToSemaphore(int id, Process_PCB * process){
    Semaphore * sem = getSemaphore(id);
    return List_prepend(sem->blockedProccesses, process);
}

void printSemaphore(int id) {
    Semaphore * sem = getSemaphore(id);
    if(sem != NULL) {
        printf("Semaphore %d\n\ts = %d\n\tblocked processs : \n", sem->id, sem->s);
        int numProcess = sem->blockedProccesses->count;
        List_first(sem->blockedProccesses);
        for(int i = 0; i < numProcess; i++){
            printProcess(*(Process_PCB *)List_curr(sem->blockedProccesses));
            List_next(sem->blockedProccesses);
        }
    }
}

void printAllSemaphores() {
    for(int i = 0; i < NUM_SEMAPHORES; i++) {
        printSemaphore(i);
    }
}

void destroyAllSemaphores() {
    for(int i = 0; i < NUM_SEMAPHORES; i++) {
        destroySemaphore(i);
    }
}
