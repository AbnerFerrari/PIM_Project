#include <pthread.h>

char* database_get(char* table_name, int id, char** entity);
void database_read(char* table, char* buffer, long buffer_size, int chunk_size, int metadata_size);
int database_write(char* table_name, char* buffer);
void database_update(char* table_name, char* buffer);
void database_delete(char* table_name, int id);
int user_exists(char *buffer);

// Retorna o tamanho do arquivo informado
long get_file_size(char* file_name);