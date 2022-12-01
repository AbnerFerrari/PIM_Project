#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../headers/constants.h"

void get_file_name_with_extension(char* table_name, char** buffer) {
    int table_name_length = strlen(table_name);
    int file_name_length = table_name_length + FILE_EXTENSION_SIZE + 1;
    *buffer = malloc(file_name_length); // The + 1 here is from null character
    bzero(*buffer, file_name_length);
    strcat(*buffer, table_name);
    strcat(*buffer, FILE_EXTENSION);
}
