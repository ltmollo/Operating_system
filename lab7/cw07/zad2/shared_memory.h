#ifndef ZAD1_SHARED_MEMORY_H
#define ZAD1_SHARED_MEMORY_H
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char *add_shared_memory(const char* filename, int size);
bool detach_shared_memory(char *shared_memory);
bool destroy_shared_memory(const char *filename);

#endif //ZAD1_SHARED_MEMORY_H
