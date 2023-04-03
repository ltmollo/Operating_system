#include "lib_change.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

long get_file_size(const char* filename){
    FILE* file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr,"Can't open a file: %s\n", filename);
        fclose(file);
        exit(1);
    }
    fseek(file, 0, SEEK_END);   // pointer at the end
    long file_size = ftell(file);
    rewind(file);
    fclose(file);
    return file_size;
};

void change_ascii( const char wanted_char, const char changed_char, const char* file_path, const char* result_path){
    long file_size = get_file_size(file_path);
    char* content = get_content(file_path, file_size);

    size_t length = strlen(content);
    char* changed_content = calloc(sizeof(char), length + 1);

    for(int i = 0; i < length; i++){
        changed_content[i] = (content[i] == wanted_char) ? changed_char : content[i];
    }

    changed_content[length] = '\0';
    write_to_file(result_path, changed_content, file_size);
    free(content);
    free(changed_content);
}

char* get_content(const char* filename, long file_size) {

    char* buffer;
    int file = open(filename, O_RDONLY);

    if (!file) {
        fprintf(stderr,"Can't open a file: %s\n", filename);
        exit(1);
    }

    long nread;
    buffer = malloc(file_size * sizeof(char));
    nread = read(file, buffer, file_size);
    if (nread == -1) {
        fprintf(stderr, "Can't open a file");
        exit(1);
    }
    close(file);

    return buffer;
}

void write_to_file(const char* result_path, char* changed_content, long file_size){
    int fd = open(result_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        fprintf(stderr, "Can't open a file\n");
        close(fd);
        exit(1);
    }
    ssize_t nwrite = write(fd, changed_content, file_size);
    if (nwrite == -1) {
        fprintf(stderr, "Can't open a file\n");
        close(fd);
        exit(1);
    }
    close(fd);
}
