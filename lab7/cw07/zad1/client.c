#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "shared_memory.h"
#include "queue_manager.h"
#include "semaphore_manager.h"

static Semaphore sem_queue;
static Semaphore sem_chairs;
static Semaphore sem_barbers;
static Semaphore buffer_mutex;

char get_random_haircut() {
    return (char) (rand() % 128);
}

int main(void) {
    srand(time(NULL) + getpid());

    char *queue = add_shared_memory(PROJECT_ID, BUFFER_SIZE);
    if (queue == NULL) {
        fprintf(stderr, "Client can't open queue.\n");
        exit(EXIT_FAILURE);
    }

    sem_queue = open_semaphore(QUEUE);
    sem_chairs = open_semaphore(CHAIRS);
    sem_barbers = open_semaphore(BARBERS);
    buffer_mutex = open_semaphore(BUFFER_MUTEX);

    if (strlen(queue) >= QUEUE_SIZE) {
        printf("CLIENT-%d \t Queue is full. Leaving.\n", getpid());
        fflush(stdout);
    }

    aquire(sem_queue);

    aquire(buffer_mutex);
    char haircut = get_random_haircut();
    printf("CLIENT-%d \t New client wants haircut no. %d \n", getpid(), haircut);
    fflush(stdout);
    queue_push(queue, haircut);
    release(buffer_mutex);

    release(sem_barbers);
    aquire(sem_chairs);

    printf("CLIENT-%d \t Client serviced.\n", getpid());
    fflush(stdout);

    detach_shared_memory(queue);

    return EXIT_SUCCESS;
}