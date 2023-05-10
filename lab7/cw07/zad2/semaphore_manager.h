#ifndef ZAD1_SEMAPHORE_MANAGER_H
#define ZAD1_SEMAPHORE_MANAGER_H

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include "variables.h"

typedef sem_t* Semaphore;

Semaphore create_semaphore(const char *filename, int value);
Semaphore open_semaphore(const char *filename);
void unlink_semaphore(const char* filename);
void aquire(Semaphore sem);
void release(Semaphore sem);
void close_semaphore(Semaphore semaphore);
void unlink_semaphores();


#endif //ZAD1_SEMAPHORE_MANAGER_H
