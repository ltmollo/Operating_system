#include "semaphore_manager.h"

#include <stdlib.h>

Semaphore create_semaphore(const char *filename, int value) {
    key_t key = ftok(HOME_PATH, filename[0]);
    if (key == -1) {
        perror("Semaphore_manager failed on ftok\n");
        return -1;
    }
    Semaphore semid = semget(key, 1, 0664 | IPC_CREAT);
    if (semid == -1) {
        perror("Semaphore_manager failed on semget\n");
        return -1;
    }
    if(semctl(semid, 0, SETVAL, value) == -1) {
        perror("Semaphore_manager failed on semctl\n");
        return -1;
    }
    return semid;
}

Semaphore open_semaphore(const char *filename) {

    key_t key = ftok(HOME_PATH, filename[0]);
    if (key == -1) {
        return -1;
    }
    return semget(key, 1, 0);
}

void unlink_semaphore(const char* filename) {
    Semaphore semid = open_semaphore(filename);
    if( semid == -1) {
        fprintf(stderr, "Semaphore_manager failed to unlink semaphore.\n");
        return;
    }
    semctl(semid, 0, IPC_RMID);
}

void aquire(Semaphore sem) {
    struct sembuf operation = { 0, -1, 0 };
    if(semop(sem, &operation, 1) == -1) {
        perror("Semaphore_manager failed to aquire\n");
    }
}

void release(Semaphore sem) {
    struct sembuf operation = { 0, 1, 0 };
    if(semop(sem, &operation, 1) == -1){
        perror("Semaphore_manager failed to release");
    }
}

void unlink_semaphores(void) {
    unlink_semaphore(QUEUE);
    unlink_semaphore(CHAIRS);
    unlink_semaphore(BARBERS);
    unlink_semaphore(BUFFER_MUTEX);
}

void create_semaphores(void) {
    create_semaphore(QUEUE, CHAIR_TOTAL);
    create_semaphore(CHAIRS, 0);
    create_semaphore(BARBERS, 0);
    create_semaphore(BUFFER_MUTEX, 1);
}
