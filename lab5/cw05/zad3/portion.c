#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 256
#define FIFO_PATH "/tmp/fifo_queue"

char write_buff[BUFF_SIZE] = "";

double function(double x){
    return 4/(x*x+1);
}

double portion_integral(double a, double b, double dx){
    double sum = 0.0;
    for(; a < b; a += dx){
        sum += function(a)*dx;
    }
    return sum;
}

int main(int argc, char* args[]) {
    if(argc != 4){
        fprintf(stderr, "[Portion] Too many args. Must be 3\n");
        return 1;
    }

    double a = strtod(args[1], NULL);
    double b = strtod(args[2], NULL);
    double dx = strtod(args[3], NULL);
    double portion = portion_integral(a, b, dx);

    size_t bytes_written = snprintf(write_buff, BUFF_SIZE, "%lf\n", portion);
    if(bytes_written == 0){
        perror("[Portion] Can't write to buff\n");
        exit(EXIT_FAILURE);
    }

    int fifo = open(FIFO_PATH, O_WRONLY);

    if(!write(fifo, write_buff, bytes_written)){
        perror("[Portion] Can't write to FIFO\n");
        exit(EXIT_FAILURE);
    }

    close(fifo);

    return 0;
}