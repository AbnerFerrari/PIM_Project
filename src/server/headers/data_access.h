#include <pthread.h>

void database_read(char* table, char* buffer, long buffer_size, int chunk_size, int metadata_size, pthread_mutex_t* mutex);
int database_write(char* table_name, char* buffer, pthread_mutex_t* mutex);
int user_exists(char *buffer, pthread_mutex_t* mutex);