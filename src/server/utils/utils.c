#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "./constants.h"

long get_file_size(char* file_name, pthread_mutex_t* mutex)
{ 
    pthread_mutex_lock(mutex);
    int length = strlen(file_name) + FILE_EXTENSION_SIZE;
    char file_name_with_extension[length];
    get_file_name_with_extension(file_name, length, file_name_with_extension);
    
    FILE* file = fopen(file_name_with_extension, "r");
    
    fseek(file, 0, SEEK_END); 
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 

    fclose(file);
    pthread_mutex_unlock(mutex);

    return size;
}

void get_file_name_with_extension(char* table_name, int table_name_length, char* buffer) {
    bzero(buffer, FILE_EXTENSION_SIZE);

    strcat(buffer, table_name);
    strcat(buffer, FILE_EXTENSION);
}
