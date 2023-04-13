#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/time.h>
#include <time.h>
#include <sys/wait.h>


#define BUFF_SIZE 256
char write_buff[BUFF_SIZE] = "";
char read_buff[BUFF_SIZE] = "";

double function(double x){
    return 4/(x*x+1);
}

double sum_result(int* pipes, int n){
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        size_t size = read(pipes[i], read_buff, BUFF_SIZE);
        read_buff[size] = 0;    // read function doesn't return \0 at the end
        result += strtod(read_buff, NULL);
    }
    return result;
}

double portion_integral(double a, double b, double dx){
    double sum = 0.0;
    for(; a < b; a += dx){
        sum += function(a)*dx;
    }
    return sum;
}

int main(int argc, char* args[]) {
    if(argc != 3){
        fprintf(stderr, "Wrong nb of args. Must be 3\n");
        return 1;
    }

    double dx = strtod(args[1], NULL);

    if(dx > 1 || dx <= 0){
        fprintf(stderr, "First arg must be from (0,1)\n");
        return 2;
    }

    int n = atoi(args[2]);
    double portion_length = 1.0/n;

    int* pipes = calloc(n, sizeof(int));


    struct timespec timespec_start, timespec_end;
    clock_gettime(CLOCK_REALTIME, &timespec_start);


    for(int i = 0; i < n; i++){
        int fd[2];
        pipe(fd);

        pid_t pid = fork();
        if(pid == 0){ // child
            close(fd[0]);                   // close reading
            double portion_sum = portion_integral(i*portion_length, (i+1)*portion_length, dx);
            size_t size = snprintf(write_buff, BUFF_SIZE, "%lf", portion_sum);
            if(!write(fd[1], write_buff, size)){
                fprintf(stderr, "Can't write integral\n");
                return 2;
            }
            exit(0);
        }

        else{  // parent
            close(fd[1]);       // close write
            pipes[i] = fd[0];
        }
    }

    while(wait(NULL) > 0);

    double result = sum_result(pipes, n);

    free(pipes);

    clock_gettime(CLOCK_REALTIME, &timespec_end);

    long time_s = timespec_end.tv_sec - timespec_start.tv_sec;
    long time_ns = timespec_end.tv_nsec - timespec_start.tv_nsec;

    if (time_ns < 0) {
        time_ns = 1000000000 + time_ns;
        time_s -= 1;
    }

    printf("Result: %lf\nn: %d\ndx: %.17f\nTime: %lds %ldns\n", result, n, dx, time_s, time_ns);

    return 0;
}