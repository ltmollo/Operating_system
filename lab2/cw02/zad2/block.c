# include "rev.h"
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024

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

void block_reverse(char* block, size_t read_size){
    for (int i = 0; i < read_size / 2; i++) {
        char temp = block[i];
        block[i] = block[read_size - i - 1];
        block[read_size - i - 1] = temp;
    }
}

void reverse(const char* file_path, const char* result_path){
    long file_size = get_file_size(file_path);

    FILE* file = fopen(file_path, "r");
    FILE* res_file = fopen(result_path, "w");

    if (file == NULL || res_file == NULL) {
        printf("Failed to open file\n");
        exit(1);
    }

    char* block = calloc(sizeof(char), BLOCK_SIZE);
    size_t read_size;
    fseek(file, -1, SEEK_END);
    file_size--;
    if(file_size - BLOCK_SIZE > 0) {
        fseek(file, -BLOCK_SIZE, SEEK_CUR);
    }
    while (file_size - BLOCK_SIZE > 0) {
        read_size = fread(block, sizeof(char), BLOCK_SIZE, file);
        block_reverse(block, read_size);
        fwrite(block, sizeof(char), read_size, res_file);
        fseek(res_file, 0, SEEK_END);
        file_size -= BLOCK_SIZE;
        fseek(file, -2*BLOCK_SIZE, SEEK_CUR);
    }
    free(block);
    if(file_size > 0) {
        fseek(file, 0, SEEK_SET);
        char* block = calloc(sizeof(char), file_size+1);  // + 1 for \n
        read_size = fread(block, sizeof(char), file_size, file);
        block_reverse(block, read_size);
        block[file_size] = '\n';
        fwrite(block, 1, file_size+1, res_file);
        free(block);
    }

    fclose(file);
    fclose(res_file);
}