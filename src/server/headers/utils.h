#include <pthread.h>

// Retorna o tamanho do arquivo informado
long get_file_size(char* file_name, pthread_mutex_t* mutex);
void get_file_name_with_extension(char* table_name, int table_name_length, char* buffer);