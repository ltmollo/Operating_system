#include "semaphore_manager.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <stdlib.h>

Semaphore create_semaphore(const char *filename, int initial) {
    Semaphore semaphore = sem_open(filename, O_CREAT | O_EXCL, 0644, initial);
    if (semaphore == SEM_FAILED) {
        fprintf(stderr, "[ERROR] Failed at semaphore %s.\n", filename);
        return NULL;
    }
    return semaphore;
}

Semaphore open_semaphore(const char *filename) {
    Semaphore semaphore = sem_open(filename, 0);
    if (semaphore == SEM_FAILED) {
        fprintf(stderr, "[ERROR] Failed at semaphore %s.\n", filename);
        return NULL;
    }
    return semaphore;
}

void close_semaphore(Semaphore semaphore) {
    sem_close(semaphore);
}

void unlink_semaphore(const char* filename) {
    sem_unlink(filename);
}

void aquire(Semaphore sem) {
    sem_wait(sem);
}

void release(Semaphore sem) {
    sem_post(sem);
}

void unlink_semaphores(void) {
    unlink_semaphore(QUEUE);
    unlink_semaphore(CHAIRS);
    unlink_semaphore(BARBERS);
    unlink_semaphore(BUFFER_MUTEX);
}
