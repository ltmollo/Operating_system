#include "shared_memory.h"
#include <stdio.h>
#include <sys/mman.h>

#include <sys/shm.h>

#include <errno.h>

static int get_shared_memory(const char* filename, int size) {
    int descriptor = shm_open(filename, O_CREAT | O_RDWR, 0644);
    if (descriptor == -1) {
        return -1;
    }
    if(ftruncate(descriptor, size) == -1) {
        perror("shared_memory failed on ftruncate\n");
        return -1;
    }
    return descriptor;
}

char *add_shared_memory(const char* filename, int size) {
    int shared_memory_id = get_shared_memory(filename, size);
    if (shared_memory_id == -1) {
        fprintf(stderr, "shared_memory can't get file descriptor\n");
        return NULL;
    }
    char *shared_memory = (char*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_id, 0);
    return shared_memory;
}

bool detach_shared_memory(char *shared_memory) {
    return (shmdt(shared_memory) != -1);
}

bool destroy_shared_memory(const char *filename) {
    return (shm_unlink(filename) != -1);
}

