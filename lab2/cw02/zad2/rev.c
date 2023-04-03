#include "rev.h"
#include <stdio.h>
#include <stdlib.h>

long get_file_size(const char* filename){
    FILE* file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "Can't open a file: %s\n", filename);
        fclose(file);
        exit(1);
    }

    fseek(file, 0, SEEK_END);   // pointer at the end

    long file_size = ftell(file);

    rewind(file);
    fclose(file);

    return file_size;
}

void reverse(const char* file_path, const char* result_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Can't open a file: %s\n", file_path);
        fclose(file);
        exit(1);
    }

    FILE *res_file = fopen(result_path, "w");
    if (!res_file) {
        fprintf(stderr, "Can't open a file: %s\n", result_path);
        fclose(file);
        exit(1);
    }

    long file_size = get_file_size(file_path);
    char *pointer;
    pointer = calloc(1, sizeof(char));
    file_size -= 2;                        // moving from \0 and space
    long i = 0;
    while(file_size != -1){
        fseek(file, file_size, SEEK_SET);       //going backwards from source file
        fseek(res_file, i, SEEK_SET);           // writing forward to result file
        fread(pointer, sizeof(char), 1, file);  // reading a char from file
        fwrite(pointer, sizeof(char), 1, res_file); // writing the char to file
        i++;
        file_size--;
    }
    char c = '\n';                                                  // adding space at the end of a file
    fwrite(&c, sizeof(char), 1, res_file);
    fclose(file);
    fclose(res_file);
    free(pointer);
}