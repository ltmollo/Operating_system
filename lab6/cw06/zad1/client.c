#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "variables.h"

key_t queue_key;
int queue_id;
int main_queue_id;
int client_id;

void list() {
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));
    setTime(messageBuffer);

    messageBuffer->type = LIST;
    messageBuffer->client_id = client_id;

    msgsnd(main_queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    free(messageBuffer);
}

void to_all(char* message){
    MessageBuffer* messageBuffer = malloc(sizeof(MessageBuffer));
    setTime(messageBuffer);

    messageBuffer->type = TO_ALL;
    messageBuffer->client_id = client_id;
    strcpy(messageBuffer->content, message);
    msgsnd(main_queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    free(messageBuffer);
}

void to_one(char* message, int other_id){
    MessageBuffer* messageBuffer = malloc(sizeof(MessageBuffer));
    setTime(messageBuffer);

    messageBuffer->type = TO_ONE;
    strcpy(messageBuffer->content, message);

    messageBuffer->client_id = client_id;
    messageBuffer->other_id = other_id;
    msgsnd(main_queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    free(messageBuffer);
}

void stop(){
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));
    setTime(messageBuffer);

    messageBuffer->type = STOP;
    messageBuffer->client_id = client_id;

    char mess[MAX_SIZE_MESSAGE];
    sprintf(mess, "Client ID: %d is leaving", messageBuffer->client_id);
    strcpy(messageBuffer->content, mess);

    msgsnd(main_queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    msgctl(queue_id, IPC_RMID, NULL);
    free(messageBuffer);
    exit(0);
}

int init() {
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));
    setTime(messageBuffer);

    messageBuffer->type = INIT;
    messageBuffer->queue_key = queue_key;

    msgsnd(main_queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    msgrcv(queue_id, messageBuffer, sizeof(MessageBuffer), 0, 0);

    int c_id = messageBuffer->client_id;

    free(messageBuffer);
    return c_id;
}

void get_message() {
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));

    while(msgrcv(queue_id, messageBuffer, sizeof(MessageBuffer), 0, IPC_NOWAIT) >= 0){
        if(messageBuffer->type == STOP){
            printf("Received stop command\n");
            stop();
        }else{
            printf("Msg from: %d has been sent at %02d:%02d:%02d:\n Message: %s\n",
                   messageBuffer->client_id,
                   messageBuffer->time_struct.tm_hour,
                   messageBuffer->time_struct.tm_min,
                   messageBuffer->time_struct.tm_sec,
                   messageBuffer->content);
        }
    }
    free(messageBuffer);
}

int main() {
    srand(time(NULL));

    queue_key = ftok(HOME_PATH, rand() % 225 + 1);
    queue_id = msgget(queue_key, IPC_CREAT | 0666);
    key_t s_key = ftok(HOME_PATH, PROJ);
    main_queue_id = msgget(s_key, 0);
    client_id = init();

    if(client_id == NO_CLIENTS){
        printf("Reached max nb of clients (%d)\n", NO_CLIENTS);
        return 0;
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