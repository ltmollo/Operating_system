#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

#define BUFF_SIZE 4096
#define FIFO_MODE 0666
#define FIFO_PATH "/tmp/fifo_queue"

char write_buff[BUFF_SIZE] = "";
char read_buff[BUFF_SIZE] = "";

char arg1[BUFF_SIZE];
char arg2[BUFF_SIZE];


double sum_result(int fifo, int n){
    double result = 0.0;
    int lines_read = 0;
    char delimiter[] = "\n";
    int flag = 0;

    while (lines_read < n){

        size_t bytes_read = read(fifo, read_buff, BUFF_SIZE);

        read_buff[bytes_read] = 0;   // read function doesn't return \0 at the end

        char* token;

        token = strtok(read_buff, delimiter);

        for (;token != NULL; token = strtok(NULL, delimiter)) { // if nothing more token == NULL
            double new_num = strtod(token, NULL);
            if(flag == 1){
                new_num /= 1000000;
                flag = 0;
                lines_read--;
            }
            else if(strlen(token) != 8){
                flag = 1;
            }

            result += new_num;
            lines_read++;
        }

    }
    return result;
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

    struct timespec timespec_start, timespec_end;
    clock_gettime(CLOCK_REALTIME, &timespec_start);

    mkfifo(FIFO_PATH, FIFO_MODE);

    for(int i = 0; i < n; i++) {
        pid_t pid = fork();
        if(pid == 0){
           size_t bytes_written1 = snprintf(arg1, BUFF_SIZE, "%lf", i*portion_length);
           size_t bytes_written2 = snprintf(arg2, BUFF_SIZE, "%lf", (i+1)*portion_length);

           if(bytes_written1 == 0 || bytes_written2 == 0){
               perror("[Main] Can't write args\n");
               exit(EXIT_FAILURE);
           }
           execl("./portion", "portion", arg1, arg2, args[1], NULL);
        }
    }

    int fifo = open(FIFO_PATH, O_RDONLY);

    double result = sum_result(fifo, n);

    close(fifo);

    remove(FIFO_PATH);

    clock_gettime(CLOCK_REALTIME, &timespec_end);

    long time_s = timespec_end.tv_sec - timespec_start.tv_sec;
    long time_ns = timespec_end.tv_nsec - timespec_start.tv_nsec;

    if (time_ns < 0) {
        time_ns = 1000000000 + time_ns;
        time_s -= 1;
    }

    printf("Result: %f\nn: %d\ndx: %.17f\nTime: %lds %ldns\n", result, n, dx, time_s, time_ns);

    return 0;
}