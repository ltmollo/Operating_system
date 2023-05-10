#include "queue_manager.h"

#include <stdio.h>
#include <string.h>

#include "variables.h"

bool queue_empty(char *queue) {
    return (strlen(queue) == 0);
}

char queue_pop(char *queue) {
    if(queue_empty(queue)) {
        return '\0';
    }
    char first = queue[0];
    memcpy(queue, queue + 1, strlen(queue) + 1);
    return first;
}

bool queue_full(char *queue) {
    return (strlen(queue) + 1 == BUFFER_SIZE);
}

void queue_push(char *queue, char byte) {
    if(queue_full(queue)) {
        fprintf(stderr, "queue_manager failed to push to queue. Queue is full.\n");
        return;
    }
    int size = strlen(queue);
    queue[size] = byte;
    queue[size + 1] = '\0';
}