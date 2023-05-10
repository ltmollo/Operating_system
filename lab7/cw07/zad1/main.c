#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "shared_memory.h"
#include "semaphore_manager.h"

int main(void) {
    printf("Barbers: %d, Chairs: %d, Queue size: %d, Clients: %d\n\n",
           BARBER_TOTAL,
           CHAIR_TOTAL,
           QUEUE_SIZE,
           CLIENTS_TOTAL);
    fflush(stdout);

    char *shared_memory = add_shared_memory(PROJECT_ID, BUFFER_SIZE);
    if(shared_memory == NULL) {
        exit(EXIT_FAILURE);
    }
    shared_memory[0] = '\0';

    unlink_semaphores();
    create_semaphores();

    //create barbers
    for(int i=0;i<BARBER_TOTAL;++i){
        pid_t pid_barb = fork();
        if(pid_barb == -1){
            perror("Main can't create new barber\n");
            exit(EXIT_FAILURE);
        }
        if (pid_barb == 0)
            execl("./barber", "./barber", NULL);
    }
    fflush(stdout);


    //create customers
    for(int i=0; i < CLIENTS_TOTAL; ++i){
        pid_t pid_client = fork();
        if(pid_client == -1){
            perror("Main can't create new client\n");
            exit(EXIT_FAILURE);
        }
        if (pid_client == 0)
            execl("./client", "./client", NULL);
    }
    fflush(stdout);

    while(wait(NULL) > 0);

    if (!destroy_shared_memory(PROJECT_ID)) {
        fprintf(stderr, "Main failed to release shared_memory memory.\n");
        exit(EXIT_FAILURE);
    }
    printf("\nSimulation finished.\n");
    fflush(stdout);
    return 0;
}