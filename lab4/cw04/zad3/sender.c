#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SIGNAL SIGUSR1

void signal_handler(int sig, siginfo_t *info, void *ucontext) {
    {
        printf("Sender received SIGNAL from catcher\n");
    }
}

void set_action(struct sigaction action) {
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = signal_handler;
    sigaction(SIGNAL, &action, NULL);
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Invalid arguments, must be at least 3\n");
        return 1;
    }

    int catcher_pid = atoi(argv[1]);
    int request;

    for (int i = 2; i < argc; i++) {
        request = atoi(argv[i]);

        if(request == 5 && i != argc - 1){
            fprintf(stderr, "Arg 5 can be only at the end of command!. Request won't be sent\n");
            continue;
        }

        if (request< 1 || request > 5) {
            fprintf(stderr, "Invalid request %d. Request won't be sent\n", request);
            continue;
        }

        struct sigaction action;
        set_action(action);

        sigval_t sig_val = {request};

        if(sigqueue(catcher_pid, SIGNAL, sig_val) == -1){
            fprintf(stderr, "Wrong catcher PID given\n");
            exit(2);
        };


        sigsuspend(&action.sa_mask);
    }
return 0;
}