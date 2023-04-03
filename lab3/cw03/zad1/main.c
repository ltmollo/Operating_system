#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[]) {
    if(argc != 2){
        fprintf(stderr, "To many args. Must be 2");
        return 1;
    }

    int number_of_process = atoi(argv[1]);

    for(int i = 0; i < number_of_process; i++){
        pid_t pid = fork();
        if(pid == -1){
            fprintf(stderr, "Can't create fork");
            return 2;
        }
        if(pid == 0){
            printf("Parent process ID: %d, Child process ID: %d \n", getppid(), getpid());
            exit(0);
        }
    }
    while (wait(NULL) > 0); // Wait for children

    printf("argv[1] = %s\n", argv[1]);
    return 0;
}
