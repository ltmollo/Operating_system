#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "shared_memory.h"
#include "queue_manager.h"
#include "semaphore_manager.h"

static Semaphore sem_queue;
static Semaphore sem_chairs;
static Semaphore sem_barbers;
static Semaphore buffer_mutex;


int main(void)
{
    char *queue = add_shared_memory(PROJECT_ID, BUFFER_SIZE);
    if (queue == NULL)
    {
        fprintf(stderr, "BARBER- can't open queue.\n");
        exit(EXIT_FAILURE);
    }

    sem_queue = open_semaphore(QUEUE);
    sem_chairs = open_semaphore(CHAIRS);
    sem_barbers = open_semaphore(BARBERS);
    buffer_mutex = open_semaphore(BUFFER_MUTEX);

    printf("BARBEER-%d \t sleeps\n", getpid());
    fflush(stdout);

    while (1)
    {

        aquire(sem_barbers);
        release(buffer_mutex);

        char haircut = queue_pop(queue);
        int time_for_haircut = haircut * 10 + USUAL_HAIRCUT_TIME;
        release(buffer_mutex);

        printf("BARBER-%d \t Processing hairuct number %d, requires %d time\n", getpid(), haircut, time_for_haircut);
        fflush(stdout);

        usleep(time_for_haircut);

        printf("BARBER-%d \t Done with hairuct number. %d\n", getpid(), haircut);
        fflush(stdout);

        release(sem_chairs);
        release(sem_queue);

        if (queue_empty(queue))
        {
            usleep(TIMEOUT);
            if (queue_empty(queue))
                break;
        }
    }
    printf("BARBER-%d \t Going sleep.\n", getpid());
    fflush(stdout);

    detach_shared_memory(queue);
    return EXIT_SUCCESS;
}

