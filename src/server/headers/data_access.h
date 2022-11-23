#include <pthread.h>

void database_read(char* table, char* buffer, long buffer_size, int chunk_size, int metadata_size);
int database_write(char* table_name, char* buffer);
int user_exists(char *buffer);

// Retorna o tamanho do arquivo informado
long get_file_size(char* file_name);