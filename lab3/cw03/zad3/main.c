#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include "limits.h"
#include <sys/stat.h>

void search_directory(char* dir_path, char* search_str);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "There must be 3 args\n");
        exit(1);
    }

    if(strlen(argv[2]) > MAX_STRING_SIZE){
        fprintf(stderr, "argv[2] must be shorter than %d", MAX_STRING_SIZE);
    }
    search_directory(argv[1], argv[2]);

    return 0;
}

void search_directory(char* dir_path, char* search_str) {
    DIR* dir;
    struct dirent* dir_entry;
    struct stat dir_stat;
    char path[MAX_PATH_LENGTH];

    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    // Browsing files in a directory
    while ((dir_entry = readdir(dir)) != NULL) {
        // Skipping dir . and ..
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
            continue;
        }

        // Creating file path
        if(snprintf(path, MAX_PATH_LENGTH, "%s/%s", dir_path, dir_entry->d_name) >= MAX_PATH_LENGTH){
            fprintf(stderr, "too long path to file\n");
            exit(EXIT_FAILURE);
        }

        // Checking type of file
        if (stat(path, &dir_stat) == -1) {
            perror("stat error");
            exit(EXIT_FAILURE);
        }

        // Creaitng new pid for a directory
        if (S_ISDIR(dir_stat.st_mode)) {
            pid_t child_pid = fork();

            if (child_pid == -1) {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
            else if (child_pid == 0) {
                search_directory(path, search_str);
                exit(EXIT_SUCCESS);
            }
            else { // Parent pid
                continue;
            }
        }
            // print file path
        else if (S_ISREG(dir_stat.st_mode)) {
            FILE* file = fopen(path, "r");

            if (file == NULL) {
                perror("fopen error");
                exit(EXIT_FAILURE);
            }

            char buffer[strlen(search_str)+1];

            if (fgets(buffer, strlen(search_str)+1, file) != NULL && strstr(buffer, search_str) != NULL) {
                printf("%s %d\n", path, getpid());
                fflush(NULL);
            }

            fclose(file);
        }
    }

    // Close dir
    if (closedir(dir) == -1) {
        perror("closedir error");
        exit(EXIT_FAILURE);
    }
    while (wait(NULL) > 0);
}