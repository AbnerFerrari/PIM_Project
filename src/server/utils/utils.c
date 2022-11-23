#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../headers/constants.h"

void get_file_name_with_extension(char* table_name, int table_name_length, char* buffer);

void get_file_name_with_extension(char* table_name, int table_name_length, char* buffer) {
    bzero(buffer, FILE_EXTENSION_SIZE);

    strcat(buffer, table_name);
    strcat(buffer, FILE_EXTENSION);
}
