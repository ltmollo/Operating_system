#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

//

void* update_grid_area(void* args) {
    UpdateGrid* grid = (UpdateGrid*) args;

    while (1) {
        for (int i = grid->begin; i < grid->end; i++) {
            grid->dst[i] = is_alive(i / grid_width, i % grid_width, grid->src);
        }
        pause();                // wait for another signal

        char* tmp = grid->src;
        grid->src = grid->dst;
        grid->dst = tmp;
    }

    return NULL;
}

void handler(int signo, siginfo_t* info, void* context) {
    // don't want segmentation fault
}

void update_grid_multithreaded(char* src, char* dst) {
    static pthread_t* threads = NULL;
    static int _n_threads = -1;

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    sigaction(SIGUSR1, &action, NULL);

    if (!threads) {
        int n_threads = grid_width*grid_height;         // thread for every cell

        threads = malloc(sizeof(pthread_t) * n_threads);
        _n_threads = n_threads;

        int block = 1;

        for (int i = 0; i < n_threads; i += block) {

            // ending the program releases memory
            UpdateGrid* args = malloc(sizeof(UpdateGrid));
            args->src = src;
            args->dst = dst;
            args->begin = i;                                            // range update
            args->end = (i+2*block > n_threads) ? n_threads : i+block;

            pthread_create(
                    threads+(i/block),
                    NULL,
                    update_grid_area,
                    (void*) args
            );
        }
    }

    for (int i=0; i < _n_threads; i++) {
        pthread_kill(threads[i], SIGUSR1);  // send signal
    }
}