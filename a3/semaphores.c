
#include "semaphores.h"

void initializeSemaphoreArray(){
    for(int i = 0; i < NUM_SEMAPHORES; i++){
        semaphores[i] = NULL;
    }
}

// Semaphore V()
// - Simply increments s
// - If s > 0, unblock the processes in the associated
// semaphore
// - Returns an unblocked process
Process_PCB * semaphoreV(int id){
    Semaphore * sem = getSemaphore(id);
    if(sem != NULL){
        sem->s++;
        printf("SEMAPHORE %d: S value after V() = %d\n", id, sem->s);
        if(sem->s > 0) {
            Process_PCB * process = dequeueFromSemaphore(id);
            if(process != NULL) {
                printf("SEMAPHORE %d: Unblocking process from semaphore %d\n", id, id);
                process->processState = ready;
                return process;
            } 
        }
    }
    return NULL;
}

// Semaphore P()
// - Decrements s only if s > 0
// - Blocks the process calling the semaphore P() if s <= 0
int semaphoreP(int id, Process_PCB * process){
    if(process == NULL){
        printf("SEMAPHORE %d: Given process is NULL\n", id);
        return FAILURE;
    }
    Semaphore * sem = getSemaphore(id);
    if(sem != NULL) {
        if(sem->s > 0) {
            sem->s--;
            printf("SEMAPHORE %d: S value after P() = %d\n", id, sem->s);
        }
        if(sem->s <= 0) {
            // Only add the process to blocking queue if it is not the initProcess
            if(process->pid != INIT_PROCESS_PID) {
                int result = queueToSemaphore(id, process);
                if(result == SUCCESS){
                    printf("SEMAPHORE %d: Blocking process with id %d\n", id, process->pid);
                    process->processState = blockedSem;
                }
                return result;
            }
            // If it's the init process, always return success as blocking does not
            // occur on that process
            else {
                return SUCCESS;
            }
        }
        return SUCCESS;
    }
    // return failure since the id of semaphore is not defined
    return FAILURE;
}

int createSemaphore(int id, int sVal) {
    if(id > NUM_SEMAPHORES - 1 || id < 0) {
        printf("SEMAPHORE %d: Unable to create semaphore due to invalid id. Given id = %d (Must be from 0 - 4)\n", id, id);
        return FAILURE;
    }
    if(semaphores[id] == NULL) {
        semaphores[id] = (Semaphore *)malloc(sizeof(Semaphore));
        semaphores[id]->id = id;
        if(sVal < 0 ) sVal = 0;
        semaphores[id]->s = sVal;
        semaphores[id]->blockedProccesses = List_create();
        printf("SEMAPHORE %d: Created new semaphore with id %d\n", id, id);
        return SUCCESS;
    }
    else{
        printf("SEMAPHORE %d: Unable to create semaphore since semaphore is already created. Given id = %d\n", id, id);
        return FAILURE;
    }
}

void destroySemaphore(int id) {
    Semaphore * sem = getSemaphore(id);
    if(sem != NULL) {
        List_free(sem->blockedProccesses, freeProcess);
        free(sem);
        sem = NULL;
        printf("SEMAPHORE %d: Destroyed semaphore with id %d\n", id, id);
    }
}

Semaphore * getSemaphore(int id) {
    if(id >= 0 && id < NUM_SEMAPHORES){
        return semaphores[id];
    }
    else{
        printf("SEMAPHORE %d: Unable to get semaphore due to invalid id. Given id = %d (Must be from 0 - 4)\n", id, id);
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
        printf("Semaphore %d:\n\t%d\n\tblocked processs : \n", sem->id, sem->s);
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
