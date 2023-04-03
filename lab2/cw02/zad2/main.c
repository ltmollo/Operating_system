#include <stdio.h>
#include "rev.h"
#include <time.h>
#include <sys/times.h>

int main(int argc, char *argv[]) {
    if(argc != 3){
        fprintf(stderr, "There must be 3 args\n");
        return 1;
    }
    argv[2][strcspn(argv[2], "\n")] = '\0';
    struct timespec t_start, t_end;
    struct tms start_times, end_times;

    double elapsed_user, elapsed_real, elapsed_system;

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    times(&start_times);                    // start cpu

    reverse( argv[1], argv[2]);

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
