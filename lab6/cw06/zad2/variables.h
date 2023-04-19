#ifndef ZAD2_VARIABLES_H
#define ZAD2_VARIABLES_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>
#include <mqueue.h>

#define SQUEUE "/SERVER"
#define NO_CLIENTS 3
#define MAX_SIZE_MESSAGE 512

typedef struct MessageBuffer {
    long type;
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

char randomC() {
    return rand() % ('Z' - 'A' + 1) + 'F';
}

mqd_t create_queue(const char* name) {
    struct mq_attr attr;
    attr.mq_maxmsg = NO_CLIENTS;
    attr.mq_msgsize = sizeof(MessageBuffer);
    return mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
}

void setTime(MessageBuffer* messageBuffer){
    time_t curr_time = time(NULL);
    messageBuffer->time_struct = *localtime(&curr_time);
}

#endif //ZAD2_VARIABLES_H
