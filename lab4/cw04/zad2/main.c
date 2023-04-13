#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define SIGNAL SIGUSR1
#define MAX_CALLS 5

int id;
int depth;

typedef void (*handler_t)(int, siginfo_t*, void*);

void set_action(struct sigaction action, handler_t handler, int flag) {
    sigemptyset(&action.sa_mask);   // add signals that will be blocked while doing handler
    action.sa_sigaction = handler;
    action.sa_flags = flag;
    sigaction(SIGNAL, &action, NULL);
    id = 0;
    depth = 0;
}

void depth_handler(int signo, siginfo_t* info, void* context) {     // test recursion
    printf("(+) id: %d, depth: %d\n", id, depth);

    id++;
    depth++;
    if (id < MAX_CALLS)
        kill(getpid(), SIGNAL);
    depth--;

    printf("(-) id: %d, depth: %d\n", id - 1, depth);
}

void info_handler(int signo, siginfo_t* info, void* context) {
    printf("Signal number: %d\n", info->si_signo);
    printf("Singal PID: %d\n", info->si_pid);
    printf("Signal UID: %d\n", info->si_uid);
    printf("Signal ERRNO: %d\n", info->si_errno);
    printf("Signal ERRNO: %d\n", info->si_errno);
    printf("Signal exit: %x\n", info->si_status);
}

void testNODEFER(struct sigaction act) {        // test recursion
    set_action(act, depth_handler, SA_NODEFER);
    kill(getpid(), SIGNAL);
}

void testSIGINFO(struct sigaction act) {
    printf( "SELF:\n" );                                    // itself
    set_action(act, info_handler, SA_SIGINFO);
    kill(getpid(), SIGNAL);
    printf("\n");

    printf("CHILD:\n");
    if (fork() == 0) {                                            // child
        kill(getpid(), SIGNAL);
        exit(0);
    }
    else
        wait(NULL);
    printf("\n");

    printf("CUSTOM:\n");                                    // custom
    set_action(act, info_handler, SA_SIGINFO);
    sigval_t sig_val = {0xABC};                            // custom exit status
    sigqueue(getpid(), SIGNAL, sig_val);
}

void testRESETHAND(struct sigaction act) {  // sigaction works 1 time, like signal
    set_action(act, depth_handler, SA_RESETHAND);
    kill(getpid(), SIGNAL);         // second time without handler
    kill(getpid(), SIGNAL);
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid arguments.\n");
        exit(1);
    }

    struct sigaction action;

    printf("--------------\n");
    printf("SIGINFO:\n");
    testSIGINFO(action);

    printf("--------------\n");
    printf("NODEFER:\n");
    testNODEFER(action);

    printf("--------------\n");
    printf("RESETHAND:\n");
    testRESETHAND(action);

    return 0;
}