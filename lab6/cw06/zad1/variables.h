#ifndef ZAD1_VARIABLES_H
#define ZAD1_VARIABLES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define HOME_PATH getenv("HOME")
#define PROJ 1
#define NO_CLIENTS 3
#define MAX_SIZE_MESSAGE 512


typedef struct MessageBuffer {
    long type;
    key_t queue_key;
    int client_id;
    int other_id;                   // send to this client
    char content[MAX_SIZE_MESSAGE];
    struct tm time_struct;
} MessageBuffer;

typedef enum {
    INIT = 1,
    LIST = 2,
    TO_ALL = 3,
    TO_ONE = 4,
    STOP = 5
} CURRENT_REQUEST;

void setTime(MessageBuffer* messageBuffer){
    time_t curr_time = time(NULL);
    messageBuffer->time_struct = *localtime(&curr_time);
}

#endif //ZAD1_VARIABLES_H
