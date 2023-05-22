#ifndef CW09_COMMON_H
#define CW09_COMMON_H

#define NB_OF_REINDEERS 9
#define NB_OF_ELVES 10
#define REQUIRED_ELVES 3

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    unsigned int elves, reindeer, nb_of_task;
    bool is_santa_awake;
} Monitor;

#endif //CW09_COMMON_H
