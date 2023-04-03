#include "library.h"

char command[MAX_BUFF_SIZE] = "";

void Result_init(Result* Result, size_t maxSize){
    Result->table = calloc(maxSize, sizeof(char*));
    Result->size = 0;
    Result->maxSize = maxSize;
}

bool range_check(size_t currSize, size_t index){
    if(currSize <= index){
        fprintf(stderr, "Index out of Range\n");
        return false;
    }
    return true;
}

void delete_free_space(Result* Result, size_t index){
    if(index == 0){
        Result->table++;
        return;
    }
    for(size_t i = index; i < Result->size - 1; i++){
        memmove(&Result[i], &Result[i+1], sizeof(char));
    }
    free(Result->table[Result->size-1]);
}

void Result_pop(Result* Result, size_t index){
    if(range_check(Result->size, index)){
        delete_free_space(Result, index);
        Result->size--;
    }
}

char* Result_get(Result* Result, size_t index){
    if(range_check(Result->size, index)){
        return Result->table[index];
    }
    return "";
}

void Result_clear(Result* Result){
    for(size_t i = 0; i < Result->size; i++){
        free(Result->table[i]);
    }
    Result->size = 0;
}

void Result_destructor(Result* Result){
    Result_clear(Result);
    free(Result);
}

char* get_content(const char* filename) {

    FILE* file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr,"Can't open a file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);   // pointer at the end
    long file_size = ftell(file);
    rewind(file);                           // pointer at the beginning

    char* content = calloc(file_size, sizeof(char));
    if (!content) {
        fprintf(stderr, "Cant allocate memory\n");
        fclose(file);
        return NULL;
    }

    size_t __attribute__((unused)) bytes_read_1 = fread(content, sizeof(char), file_size, file); // don't want warning

    fclose(file);

    return content;
}

void Result_push(Result* Result, char* input_file){
    char temp_file[] = "/tmp/file_XXXXXX";
    int fd = mkstemp(temp_file);                 // create new file

    if (fd == 0) {
        fprintf(stderr, "Can't create temp file\n");
        return;
    }

    snprintf(command, MAX_BUFF_SIZE, "wc '%s' 1> '%s'", input_file, temp_file);
    size_t __attribute__((unused)) bytes_read_2 = system(command);                           // run the command

    char* wc_result = get_content(temp_file);

    snprintf(command, MAX_BUFF_SIZE, "rm -f '%s' 2>/dev/null", temp_file);
    size_t __attribute__((unused)) bytes_read_3 = system(command);

    if(strlen(wc_result) == 0){
        fprintf(stderr, "Failed to read temp file\n");
        return;
    }

    if(range_check(Result->maxSize, Result->size)){
        Result->table[Result->size] = wc_result;
        Result->size++;
    }
    else{
        fprintf(stderr, "Not enough memory\n");
    }
}