#ifndef ZAD1_QUEUE_MANAGER_H
#define ZAD1_QUEUE_MANAGER_H

#include <stdbool.h>

char queue_pop(char *queue);
void queue_push(char *queue, char byte);
bool queue_full(char *queue);
bool queue_empty(char *queue);


#endif //ZAD1_QUEUE_MANAGER_H
