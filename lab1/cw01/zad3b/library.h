#ifndef CW01_LIBRARY_H
#define CW01_LIBRARY_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUFF_SIZE 2048

typedef struct {
    char** table;   // strings array
    size_t size;    // current size
    size_t maxSize; // max size
} Result ;

void Result_init(Result* result, size_t maxSize);

void Result_push(Result* Result, char* input);

void Result_pop(Result* Result, size_t index);

char* Result_get(Result* Result, size_t index);

void Result_clear(Result* Result);

void Result_destructor(Result* Result);
#endif //CW01_LIBRARY_H
