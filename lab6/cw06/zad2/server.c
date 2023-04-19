#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "variables.h"

mqd_t main_queue;
char *clients_queue[NO_CLIENTS];

void write_to_file();

void list(MessageBuffer messageBuffer){
    char mess[MAX_SIZE_MESSAGE] = "ID of active clients:";
    strcpy(messageBuffer.content, mess);
    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] != NULL){
            sprintf(mess + strlen(mess), " %d", i);
        }
    }

    sprintf(mess + strlen(mess), "\n");
    strcpy(messageBuffer.content, mess);

    mqd_t cQueueDesc = mq_open(clients_queue[messageBuffer.client_id], O_RDWR);
    mq_send(cQueueDesc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_close(cQueueDesc);
    write_to_file(&messageBuffer);

}

void to_all(MessageBuffer messageBuffer){

    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] == NULL) {
            continue;
        }
        mqd_t other_client_queue_desc = mq_open(clients_queue[i], O_RDWR);
        mq_send(other_client_queue_desc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
        mq_close(other_client_queue_desc);
    }
    write_to_file(&messageBuffer);

}

void to_one(MessageBuffer messageBuffer){

    mqd_t otherQueueDesc = mq_open(clients_queue[messageBuffer.other_id], O_RDWR);

    mq_send(otherQueueDesc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_close(otherQueueDesc);
    write_to_file(&messageBuffer);
}

void stop(int client_id){
    MessageBuffer *messageBuffer;

    mqd_t informQueueDesc = mq_open(clients_queue[client_id], O_RDWR);

    //to give client information he can't join if reached NO_CLTIENTS
    mq_send(informQueueDesc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_close(informQueueDesc);
    clients_queue[client_id] = NULL;

}

void handler(){
    MessageBuffer messageBuffer;
    strcpy(messageBuffer.content, "SERVER CLOSED With CTRL + C");
    setTime(&messageBuffer);
    messageBuffer.type = STOP;

    write_to_file(&messageBuffer);
    for(int i = 0; i < NO_CLIENTS; i++){
        if(clients_queue[i] == NULL) {
            continue;
        }
        messageBuffer.type = STOP;
        mqd_t otherQueueDecs = mq_open(clients_queue[i], O_RDWR);

        mq_send(otherQueueDecs, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
        mq_receive(main_queue, (char *) &messageBuffer, sizeof(MessageBuffer), NULL);
        mq_close(otherQueueDecs);
    }
    mq_close(main_queue);
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

void init(MessageBuffer messageBuffer) {

    int first_available_ID = 0;
    while (clients_queue[first_available_ID] != NULL && first_available_ID < NO_CLIENTS - 1) {
        first_available_ID++;
    }


    if (clients_queue[first_available_ID] != NULL && first_available_ID == NO_CLIENTS - 1) {
        messageBuffer.client_id = NO_CLIENTS;

    } else {
        messageBuffer.client_id = first_available_ID;
        clients_queue[first_available_ID] = (char *) calloc(NO_CLIENTS, sizeof(char));
        strcpy(clients_queue[first_available_ID], messageBuffer.content);
    }

    mqd_t cQueueDesc = mq_open(messageBuffer.content, O_RDWR);
    mq_send(cQueueDesc, (char *) &messageBuffer, sizeof(MessageBuffer), 0);
    mq_close(cQueueDesc);
    write_to_file(&messageBuffer);
}

int main() {
    for(int i = 0; i < NO_CLIENTS; i++){
        clients_queue[i] = NULL;
    }

    mq_unlink(SQUEUE);
    main_queue = create_queue(SQUEUE);

    signal(SIGINT, handler);
    MessageBuffer messageBuffer;

    while(1){

        mq_receive(main_queue, (char *) &messageBuffer, sizeof(MessageBuffer), NULL);

        switch(messageBuffer.type) {
            case INIT:
                init(messageBuffer);
                break;
            case LIST:
                list(messageBuffer);
                break;
            case TO_ALL:
                to_all(messageBuffer);
                break;
            case TO_ONE:
                to_one(messageBuffer);
                break;
            case STOP:
                stop(messageBuffer.client_id);
                write_to_file(&messageBuffer);
                break;
            default:
                printf("Wrong request\n");
        }
    }


    return 0;
}
