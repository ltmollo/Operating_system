#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define SIGNAL SIGUSR1

typedef enum {
    PRINT_NUM = 1,
    PRINT_CURR_TIME = 2,
    PRINT_CHANGE = 3,
    PRINT_TIME_LOOP = 4,
    EXIT = 5,
} CURRENT_REQUEST;

CURRENT_REQUEST request = PRINT_NUM;
int change_request = 0;
bool request_finished = true;

void print_num(){
    for(int i = 0; i < 101; i++){
        printf("%d\n", i);
    }
}

void print_curr_time(){
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    printf("Current time: %s", asctime(timeinfo));
}

void print_change(){
    printf("Change Requests: %d\n", change_request);
}

void print_time_loop(){
    print_curr_time();
    sleep(1);
}

void exit_(){
    printf("Exit program\n");
    fflush(NULL);
    exit(0);
}

void handler(int sig_number, siginfo_t* info, void* context) {
    int pid = info->si_pid;
    int request_given = info->si_status;


    change_request += 1;
    request  = (CURRENT_REQUEST) request_given;
    request_finished = false;


    kill(pid, SIGNAL);
}

void set_action(struct sigaction action) {
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGNAL, &action, NULL);
}

int main() {
    struct sigaction action;
    set_action(action);

    printf("Catcher starting, PID = %d\n", getpid());
    while (true) {
        if (request_finished)
            continue;


        switch (request) {
            case PRINT_NUM:
                print_num();
                request_finished = true;
                break;
            case PRINT_CURR_TIME:
                print_curr_time();
                request_finished = true;
                break;
            case PRINT_CHANGE:
                print_change();
                request_finished = true;
                break;
            case PRINT_TIME_LOOP:
                request_finished = true;    // if finished == false it means we
                while(request_finished){    // have a new request
                    print_time_loop();
                }
                break;
            case EXIT:
                request_finished = true;
                exit_();
                break;

        }


    }
    return 0;
}