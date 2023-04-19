#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <signal.h>

#include "variables.h"

key_t clients_queue[NO_CLIENTS];
int main_queue;
MessageBuffer *message;

void write_to_file();

void list(MessageBuffer* messageBuffer){
    char mess[MAX_SIZE_MESSAGE] = "ID of active clients:";
    strcpy(messageBuffer->content, mess);
    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] != -1){
            sprintf(mess + strlen(mess), " %d", i);
        }
    }
    sprintf(mess+ strlen(mess), "\n");
    strcpy(messageBuffer->content, mess);
    int queue_id = msgget(clients_queue[messageBuffer->client_id], 0);

    msgsnd(queue_id, messageBuffer, sizeof(MessageBuffer), 0);
}

void to_all(MessageBuffer* messageBuffer){

    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] == messageBuffer->client_id) {
            continue;
        }
        int queue_id = msgget(clients_queue[i], 0);

        msgsnd(queue_id, messageBuffer, sizeof(MessageBuffer), 0);
    }
}

void to_one(MessageBuffer* messageBuffer){

    int queue_id = msgget(clients_queue[messageBuffer->other_id], 0);

    msgsnd(queue_id, messageBuffer, sizeof(MessageBuffer), 0);
}

void stop(int client_id){
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));

    int queue_id = msgget(clients_queue[client_id], 0);

    msgsnd(queue_id, messageBuffer, sizeof(MessageBuffer), 0);

    clients_queue[client_id] = -1;

    free(messageBuffer);
}

void handler(){
    MessageBuffer *messageBuffer = malloc(sizeof(MessageBuffer));
    strcpy(messageBuffer->content, "SERVER CLOSED With CTRL + C");
    setTime(messageBuffer);
    messageBuffer->type = STOP;

    write_to_file(messageBuffer);
    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] == -1) {
            continue;
        }
        messageBuffer->type = STOP;
        int queue_id = msgget(clients_queue[i], 0);

        msgsnd(queue_id, messageBuffer, sizeof(MessageBuffer), 0);
        msgrcv(main_queue, messageBuffer, sizeof(MessageBuffer), STOP , 0);
    }
    msgctl(main_queue, IPC_RMID, NULL);         // delete main_queue
    free(messageBuffer);
    free(message);
    exit(0);

}

void write_to_file(MessageBuffer* messageBuffer){

    FILE* file = fopen("history.txt", "a");

    if(!file){
        perror("Can't open history.txt\n");
        exit(EXIT_FAILURE);
    }

    switch (messageBuffer->type) {
        case INIT:
            fprintf(file, "New client arrived\n");
            break;
        case LIST:
            fprintf(file, "Command: LIST ; Client ID: %d\nMessage: %s", messageBuffer->client_id, messageBuffer->content);
            break;
        case TO_ALL:
            fprintf(file, "Command: 2All ; Client ID: %d\n", messageBuffer->client_id);
            fprintf(file, "Message: %s\n", messageBuffer->content);
            break;
        case TO_ONE:
            fprintf(file, "Command: 2ONE ; Client ID: %d, Other ID %d\n", messageBuffer->client_id, messageBuffer->other_id);
            fprintf(file, "Message: %s\n", messageBuffer->content);
            break;
        case STOP:
            fprintf(file, "Command: STOP ; Client ID: %d\n", messageBuffer->client_id);
            fprintf(file, "Message: %s\n", messageBuffer->content);
            break;
    }
    fprintf(file, "sent at: %02d:%02d:%02d\n\n\n",
            messageBuffer->time_struct.tm_hour,
            messageBuffer->time_struct.tm_min,
            messageBuffer->time_struct.tm_sec);

    fclose(file);
}

void init(MessageBuffer* messageBuffer) {

    int first_available_ID = 0;
    while (clients_queue[first_available_ID] != -1 && first_available_ID < NO_CLIENTS - 1) {
        first_available_ID++;
    }


    if (clients_queue[first_available_ID] != -1 && first_available_ID == NO_CLIENTS - 1) {
        messageBuffer->client_id = NO_CLIENTS;

    } else {
        messageBuffer->client_id = first_available_ID;
        clients_queue[first_available_ID] = messageBuffer->queue_key;
    }
    int queue_id = msgget(messageBuffer->queue_key, 0);
    msgsnd( queue_id, messageBuffer,  sizeof(MessageBuffer), 0);
    write_to_file(messageBuffer);
}

int main() {
    for(int i = 0; i < NO_CLIENTS; i++){
        clients_queue[i] = -1;
    }

    key_t queue_key = ftok(HOME_PATH, PROJ);
    main_queue = msgget(queue_key, IPC_CREAT | 0666);

    message = malloc(sizeof(MessageBuffer));            //free in handler

    signal(SIGINT, handler);

    while(1){

        msgrcv(main_queue, message, sizeof(MessageBuffer), 0, 0); // first with - value

        switch(message->type) {
            case INIT:
                init(message);
                break;
            case LIST:
                list(message);
                write_to_file(message);
                break;
            case TO_ALL:
                to_all(message);
                write_to_file(message);
                break;
            case TO_ONE:
                to_one(message);
                write_to_file(message);
                break;
            case STOP:
                stop(message->client_id);
                write_to_file(message);
                break;
            default:
                printf("Wrong request\n");
        }
    }
    return 0;
}