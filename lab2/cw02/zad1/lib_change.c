#include "lib_change.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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

    FILE* file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr,"Can't open a file: %s\n", filename);
        fclose(file);
        exit(1);
    }

    char* content = calloc(file_size, sizeof(char));
    if (!content) {
        fprintf(stderr, "Cant allocate memory\n");
        fclose(file);
        exit(1);
    }

    size_t __attribute__((unused)) bytes_read = fread(content, sizeof(char), file_size, file); // don't want warning

    fclose(file);

    return content;
}

void write_to_file(const char* result_path, char* changed_content, long file_size){
    FILE *fp;
    fp = fopen(result_path, "w");
    if (fp == NULL) {
        fprintf(stderr, "Can't create file'\n");
        fclose(fp);
        exit(1);
    }
    size_t bytes_written = fwrite(changed_content, sizeof(char), file_size, fp);
    if (bytes_written != file_size) {
        fprintf(stderr, "Error writing to file: %s\n", result_path);
        fclose(fp);
        exit(1);
    }
    fclose(fp);
}