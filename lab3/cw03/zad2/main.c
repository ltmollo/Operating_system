#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if(argc != 2){
        fprintf(stderr, "Must be 2 args\n");
        return 1;
    }

    printf("%s ", argv[0]);
    fflush(stdout);
    execl("/bin/ls", "ls", argv[1], NULL);

    return 0;
}
