#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdbool.h>
#include "common.h"
#include <string.h>

Monitor monitor = {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .condition = PTHREAD_COND_INITIALIZER,
        .reindeer = 0,
        .elves = 0,
        .nb_of_task = 0,
        .is_santa_awake = false
};

pthread_barrier_t reindeer_barrier, elves_barrier;

int currentProblem[REQUIRED_ELVES];
int presents_Delivered = 0;

pid_t get_thread_id() {
    return syscall(__NR_gettid);
}

void initialize_current_problem(){
    for(int i=0; i<REQUIRED_ELVES; i++){
        currentProblem[i] = -1;
    }
}

void add_problem_to_solve(int problem_id){
    monitor.nb_of_task++;
    for(int i=0; i < REQUIRED_ELVES; i++){
       if(currentProblem[i] == -1){
           currentProblem[i] = problem_id;
           return;
       }
    }
}

int get_problem_to_solve(){
    monitor.nb_of_task--;
    int problem = currentProblem[0];

    for(int i=0; i<REQUIRED_ELVES-1; i++){
        currentProblem[i] = currentProblem[i+1];
    }
    currentProblem[REQUIRED_ELVES-1] = -1;
    return problem;
}

void *reindeer_routine(void *arg) {
    while (true) {
        printf("Reindeer [%d] is on vacation \n", get_thread_id());
        sleep(5 + (rand() % 6));

        pthread_mutex_lock(&monitor.mutex);
        monitor.reindeer++;

        if (monitor.reindeer == NB_OF_REINDEERS) {
            monitor.nb_of_task++;
            if(!monitor.is_santa_awake){
                printf("Reindeer [%d] wakes up Santa.\n", get_thread_id());
                monitor.is_santa_awake = true;
            }
            else{
                printf("Reindeer [%d] Santa is awake.\n", get_thread_id());
            }
        }
        else {
            printf("Reindeer [%d] came back from vacation\n", get_thread_id());
            printf("Reindeer [%d] with %d reindeers waiting for Santa \n", get_thread_id(), monitor.reindeer-1);
        }
        pthread_cond_signal(&monitor.condition);        //send signal for waiting thread
        pthread_mutex_unlock(&monitor.mutex);

        pthread_barrier_wait(&reindeer_barrier);
    }
}

void *elf_routine(void *arg) {
    while(true) {
        sleep(2 + (rand() % 4));

        pthread_mutex_lock(&monitor.mutex);

        if (monitor.elves < REQUIRED_ELVES) {
            monitor.elves += 1;
            add_problem_to_solve(get_thread_id());
            if (monitor.elves == REQUIRED_ELVES) {
                if(!monitor.is_santa_awake){
                    printf("Elf [%d] wakes up Santa\n", get_thread_id());
                    monitor.is_santa_awake = true;
                }
                else{
                    printf("Elf [%d] Santa is awake\n", get_thread_id());
                }
            }
            else {
                printf("Elf [%d] is waiting for Santa.\n", get_thread_id());
            }
            pthread_cond_signal(&monitor.condition);
        }
        else {
            printf("Elf [%d] solves the problem on their own.\n", get_thread_id());
            pthread_mutex_unlock(&monitor.mutex);
            continue;
        }
        pthread_mutex_unlock(&monitor.mutex);
        pthread_barrier_wait(&elves_barrier);

    }
}

void initialize_threads(pthread_t *reindeer_ids, pthread_t *elf_ids){
    for(int i=0;i<NB_OF_REINDEERS;i++) {
        pthread_create(&reindeer_ids[i], NULL, reindeer_routine, NULL);
    }

    for (int i=0;i<NB_OF_ELVES;i++) {
        pthread_create(&elf_ids[i], NULL, elf_routine, NULL);
    }
}

int main(int argc, char *argv[]) {

    // initialize barriers
    pthread_barrier_init(&reindeer_barrier, NULL, NB_OF_REINDEERS + 1);
    pthread_barrier_init(&elves_barrier, NULL, REQUIRED_ELVES + 1);

    //initialize problem table
    initialize_current_problem();

    // create threads
    pthread_t reindeer_ids[NB_OF_REINDEERS];
    pthread_t elf_ids[NB_OF_ELVES];

    initialize_threads(reindeer_ids, elf_ids);

    while(true) {
        pthread_mutex_lock(&monitor.mutex);
        while(monitor.reindeer < NB_OF_REINDEERS && monitor.elves < REQUIRED_ELVES) {
            pthread_cond_wait(&monitor.condition, &monitor.mutex);
        }
        if (monitor.elves == REQUIRED_ELVES) {
            for(int i=0; i<REQUIRED_ELVES; i++){
                printf("Elf [%d] meets Santa. Solving problem\n", get_problem_to_solve());
                sleep(1 + rand() % 2);
            }
            monitor.elves = 0;
            pthread_barrier_wait(&elves_barrier);
        }
        else if (monitor.reindeer == NB_OF_REINDEERS) {
            printf("Santa is delivering gifts.\n");
            sleep(2 + rand() % 3);
            monitor.nb_of_task--;
            monitor.reindeer = 0;
            presents_Delivered++;
            pthread_barrier_wait(&reindeer_barrier);
        }

        else {
            printf("Something went wrong!\n");
        }
        pthread_mutex_unlock(&monitor.mutex);
        if(monitor.nb_of_task == 0){
            printf("Santa goes to sleep\n");
            monitor.is_santa_awake = false;
            if (argc > 1 && strcmp(argv[1], "testing") == 0){
                return 0;
            }
        }
        if(presents_Delivered == 3){
            return 0;
        }
    }
    return 0;
}