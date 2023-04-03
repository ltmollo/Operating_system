#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "lib_change.h"
#include <time.h>
#include <sys/times.h>

int main(int argc, char *argv[]) {
    if(argc != 5){
        fprintf(stderr, "You can pass only 4 args\n");
        exit(1);
    }

    argv[4][strcspn(argv[4], "\n")] = '\0';

    struct timespec t_start, t_end;
    struct tms start_times, end_times;

    double elapsed_user, elapsed_real, elapsed_system;

    if(strlen(argv[1]) != 1){
        fprintf(stderr, "Wrong first arg\n");
        return 1;
    }

    if(strlen(argv[2]) != 1){
        fprintf(stderr, "Wrong second arg\n");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    times(&start_times);                    // start cpu

    change_ascii(argv[1][0], argv[2][0], argv[3],  argv[4]);

    clock_gettime(CLOCK_MONOTONIC, &t_end);
    times(&end_times);                      // end cpu

    elapsed_real = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_nsec - t_start.tv_nsec) / 1000000000;
    elapsed_user = (double)(end_times.tms_cutime - start_times.tms_cutime) / CLOCKS_PER_SEC;
    elapsed_system = (double)(end_times.tms_cstime - start_times.tms_cstime) / CLOCKS_PER_SEC;

    printf("Real time: %lf\n", elapsed_real);
    printf("User time: %lf\n", elapsed_user);
    printf("System time: %lf\n\n", elapsed_system);

    return 0;
}
