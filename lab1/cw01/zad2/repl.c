#ifdef USE_DYNAMIC_LIBRARY
#include "library_so.h"
#else
#include "library.h"
#endif

#include "dl_manager.h"
#include <regex.h>
#include <time.h>
#include <sys/times.h>

typedef enum{
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
    STOP,
    WRONG,
} COMMAND_TYPE;

regex_t init;
regex_t count;
regex_t show;
regex_t delete;
regex_t destroy;
regex_t stop;

COMMAND_TYPE command_type;

char buff[MAX_BUFF_SIZE] = "";
int number;
char* file_path;
Result* result;
bool is_initted = false;

void compile_regex(){
    regcomp(&init, "init [0-9][0-9]*", 0);                  // [0-9]+ doesn't work
    regcomp(&show, "show [0-9][0-9]*", 0);
    regcomp(&delete, "delete index [0-9][0-9]*", 0);
    regcomp(&count, "count ..*", 0);
    regcomp(&destroy, "destroy", 0);
    regcomp(&stop, "stop", 0);
}

void get_file_path(){
    file_path = buff + strlen("count ");
    file_path[strcspn(file_path, "\n")] = '\0';
}

void get_right_command(){
    if(regexec(&init, buff, 0, NULL, 0 ) == 0){
        command_type = INIT;
        number = atoi(buff + strlen("init"));
    }else if(regexec(&show, buff, 0, NULL, 0) == 0){
        command_type = SHOW;
        number = atoi(buff + strlen("show"));
    }else if(regexec(&delete, buff, 0, NULL, 0) == 0){
        command_type = DELETE;
        number = atoi(buff + strlen("delete"));
    }else if(regexec(&count, buff, 0, NULL, 0) == 0){
        command_type = COUNT;
        get_file_path();
    }else if(regexec(&destroy, buff, 0, NULL, 0) == 0){
        command_type = DESTROY;
    }else if(regexec(&stop, buff, 0, NULL, 0) == 0){
        command_type = STOP;
    }
    else{
        command_type = WRONG;
    }
}

void execute_command(){
    switch (command_type) {
        case(INIT) :
            if(!is_initted){
                if(number <= 0){
                    fprintf(stderr, "Size must by > 0\n");
                    break;
                }
                result = malloc(sizeof(Result));
                is_initted = true;
                Result_init(result, number);
            }else{
                fprintf(stderr, "You have to destroy first to init new \n");
            }
            break;
        case(COUNT):
            if(!is_initted){
                fprintf(stderr, "You have to init first\n");
            }else{
                Result_push(result, file_path);
            }
            break;
        case(SHOW):
            if(!is_initted){
                fprintf(stderr, "You have to init first\n");
            }else{
                printf("%s", Result_get(result, number));
            }
            break;
        case(DELETE):
            if(!is_initted){
                fprintf(stderr, "You have to init first\n");
            }
            else{
                Result_pop(result, number);
            }
            break;
        case(DESTROY):
            if(!is_initted){
                fprintf(stderr, "You have to init first\n");
            }
            else{
                Result_destructor(result);
                is_initted = false;
            }
            break;
        case(STOP):
            fprintf(stderr, "System will close\n");
            if(is_initted){
                Result_destructor(result);
                is_initted = false;
            }
            break;
        default:
            fprintf(stderr, "Wrong command\n");
            break;
    }
}

int main() {
    load_dynamic_library();
    compile_regex();
    struct timespec t_start, t_end;
    struct tms start_times, end_times;

    double elapsed_user, elapsed_real, elapsed_system;

    while(command_type != STOP) {
        printf("Enter command >>> ");
        char *unused_result __attribute__((unused)) = fgets(buff, MAX_BUFF_SIZE, stdin);

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        times(&start_times);                    // start cpu


        get_right_command();
        execute_command();

        clock_gettime(CLOCK_MONOTONIC, &t_end);
        times(&end_times);                      // end cpu

        elapsed_real = (double)(t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_nsec - t_start.tv_nsec) / 1000000000;
        elapsed_user = (double)(end_times.tms_cutime - start_times.tms_cutime) / CLOCKS_PER_SEC;
        elapsed_system = (double)(end_times.tms_cstime - start_times.tms_cstime) / CLOCKS_PER_SEC;

        printf("Real time: %lf\n", elapsed_real);
        printf("User time: %lf\n", elapsed_user);
        printf("System time: %lf\n\n", elapsed_system);
    }
    return 0;
}