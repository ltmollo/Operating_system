#ifndef ZAD1_LIB_CHANGE_H
#define ZAD1_LIB_CHANGE_H

#define MAX_BUFF_SIZE 1024

void change_ascii(char wanted_char, char changed_char, const char* file_path, const char* result_path);

char* get_content(const char* file_path, long file_size);

void write_to_file(const char* file_path, char* change_content, long file_size);

#endif //ZAD1_LIB_CHANGE_H
