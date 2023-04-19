#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include "variables.h"


int client_id;
char queue_name[NO_CLIENTS];
mqd_t queue_desc;
mqd_t s_queue_desc;


void list() {
    MessageBuffer messageBuffer;
    setTime(&messageBuffer);

    messageBuffer.type = LIST;
    messageBuffer.client_id = client_id;

    mq_send(s_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
}

void to_all(char* message){
    MessageBuffer messageBuffer;
    setTime(&messageBuffer);

    messageBuffer.type = TO_ALL;
    messageBuffer.client_id = client_id;
    strcpy(messageBuffer.content, message);
    mq_send(s_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
}

void to_one(char* message, int other_id){
    MessageBuffer messageBuffer;
    setTime(&messageBuffer);

    messageBuffer.type = TO_ONE;
    strcpy(messageBuffer.content, message);

    messageBuffer.client_id = client_id;
    messageBuffer.other_id = other_id;
    mq_send(s_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
}

void stop(){
    MessageBuffer messageBuffer;
    setTime(&messageBuffer);

    messageBuffer.type = STOP;
    messageBuffer.client_id = client_id;

    char mess[MAX_SIZE_MESSAGE];
    sprintf(mess, "Client ID: %d is leaving", messageBuffer.client_id);
    strcpy(messageBuffer.content, mess);

    mq_send(s_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_close(s_queue_desc);
    exit(0);
}

int init() {
    MessageBuffer messageBuffer;
    setTime(&messageBuffer);

    messageBuffer.type = INIT;
    strcpy(messageBuffer.content, queue_name);

    mq_send(s_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_receive(queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), NULL);

    int c_id = messageBuffer.client_id;

    return c_id;
}

void get_message() {
    MessageBuffer messageBuffer;

    struct timespec my_time;
    clock_gettime(CLOCK_REALTIME, &my_time);
    my_time.tv_sec += 0.1;
    while(mq_timedreceive(queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), NULL, &my_time) != -1){
        if(messageBuffer.type == STOP){
            printf("Received stop command\n");
            stop();
        }else{
            printf("Msg from: %d has been sent at %02d:%02d:%02d:\n Message: %s\n",
                   messageBuffer.client_id,
                   messageBuffer.time_struct.tm_hour,
                   messageBuffer.time_struct.tm_min,
                   messageBuffer.time_struct.tm_sec,
                   messageBuffer.content);
        }
    }
}

void get_new_name() {
    queue_name[0] = '/';

    for (int i = 1; i < NO_CLIENTS; i++) {
        queue_name[i] = randomC();
    }
}

int main() {
    srand(time(NULL));

    get_new_name();
    queue_desc = create_queue(queue_name);
    s_queue_desc = mq_open(SQUEUE, O_RDWR);

    client_id = init();
    if(client_id == NO_CLIENTS){
        printf("Reached max nb of clients (3)\n");
        return 1;
    }

    signal(SIGINT, stop);

    size_t len = 0;
    ssize_t read;
    char* command = NULL;

    while(1) {
        printf("Enter command: ");
        read = getline(&command, &len, stdin);
        command[read - 1] = '\0';

        get_message();

        if (strcmp(command, "") == 0) {
            continue;
        }

        char cmd[5];

        strncpy(cmd, command, 4);
        cmd[4] = '\0';

        if (strcmp(cmd, "LIST") == 0) {
            list();

        } else if (strcmp(cmd, "2ALL") == 0) {

            if (strlen(command) < strlen("2ALL 0")){
                printf("USAGE: 2ALL <string>\n");
                continue;
            }
            to_all(command+5);

        } else if (strcmp(cmd, "2ONE") == 0) {


            char *endptr;
            int other_id = strtol(command+4, &endptr, 10);

            if (endptr == command+4) {
                printf("USAGE: 2ONE id <string>\n");
                continue;
            }

            to_one(endptr+1, other_id);
        } else if (strcmp(cmd, "STOP") == 0) {
            stop();

        } else {
            printf("Command not recognized!\n");
        }

    }
    return 0;
}