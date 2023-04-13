#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <wait.h>

#define SINGAL_ID SIGUSR1

int is_parent = 1;

enum TASK_TYPE
{
    IGNORE,
    HANDLER,
    MASK,
    PENDING
};

enum TASK_TYPE parse_type(const char *type, int parent){
    is_parent = parent;                             // Just for execl
    if (strcmp(type, "ignore")  == 0) return IGNORE;
    if (strcmp(type, "handler") == 0) return HANDLER;
    if (strcmp(type, "mask")    == 0) return MASK;
    if (strcmp(type, "pending") == 0) return PENDING;
    fprintf(stderr, "Usage: %s <ignore|handler|mask|pending>\n", type);
    exit(1);
}

void signal_handler(int sig_number) {
    printf(" Received signal - %d.\n", getpid()); // We can distinguish parent and child pid
}

void print_is_pending() {
    sigset_t sigset;
    sigpending(&sigset);
    if (sigismember(&sigset, SIGUSR1))
        printf("Signal pending - %d\n", getpid());
    else
        printf("Signal not pending -%d.\n", getpid());
}

void block_signal() {
    struct sigaction action;
    sigemptyset(&action.sa_mask);       // set empty set
    sigaddset(&action.sa_mask, SINGAL_ID);  // add signal
    sigprocmask(SIG_BLOCK, &action.sa_mask, NULL);  // block singals in set
}

void raise_signal() {
    printf("Raising signal -%d.\n", getpid());
    raise(SINGAL_ID);
}

void process(const enum TASK_TYPE type){
    switch (type)
    {
        case IGNORE:
            // We ignore the signal
            signal(SINGAL_ID, SIG_IGN);
            raise_signal();
            break;
        case HANDLER:
            #ifdef EXEC
                fprintf(stderr, "Handler is invalid in exec version\n");
                exit(3);
            #endif
            signal(SINGAL_ID, signal_handler);
            raise_signal();
            break;
        case MASK:
            signal(SINGAL_ID, signal_handler);
            if (is_parent) {    // to see if blocking parent signal has affect on child
                block_signal();
                raise_signal();
            }
            break;
        case PENDING:
            signal(SINGAL_ID, signal_handler);
            if (is_parent) {
                block_signal();
                raise_signal();
            }
            print_is_pending();
            break;
        default:
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[2], "change_to_child_process") != 0) {
        printf("Usage: %s <ignore|handler|mask|pending>\n", argv[0]);
        exit(2);
    }

    if (argc != 2 && argc != 3) {
        printf("Usage: %s <ignore|handler|mask|pending>\n", argv[0]);
        exit(2);
    }

    const enum TASK_TYPE type = parse_type(argv[1], argc%3 );

    process(type);
    fflush(NULL);

#ifdef EXEC
    if (is_parent){
            execl(argv[0], argv[0], argv[1], "change_to_child_process", NULL);
    }
#else
    is_parent = fork();
    if (!is_parent)
        process(type);
    else
        wait(NULL);
#endif

    fflush(NULL);

    return 0;
}