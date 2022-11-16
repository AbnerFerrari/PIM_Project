#include "../headers/constants.h"
#include "../../types/funcionario.h"
#include <pthread.h>
#include <stdio.h>

void database_read(char* table, char* buffer, long buffer_size, int chunk_size, int metadata_size, pthread_mutex_t* mutex){
    int length = strlen(table) + strlen(FILE_EXTENSION);
    char file_name[length];
    get_file_name_with_extension(table, length, file_name);

    // Critical Session
    pthread_mutex_lock(mutex);

    FILE* file = fopen(file_name, "r");
    
    fseek(file, 0, SEEK_END); 
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 

    // buffer = malloc(size);
    Funcionario func = {};
    //fread(buffer, buffer_size, 1, file);
    int times = buffer_size / chunk_size;
    while(fread(&func, chunk_size, 1, file) != 0){
        char local_buffer[chunk_size + metadata_size];
        sprintf(local_buffer, FUNCIONARIO_FORMAT_OUT, func.nome, func.cpf, func.senha);
        strcat(buffer, local_buffer);
    }
    
    fclose(file);

    pthread_mutex_unlock(mutex);
    // End Critical Session
}

int user_exists(char buffer, pthread_mutex_t* mutex)
{
    // Critical Session
    pthread_mutex_lock(mutex);

    FILE* file = fopen("funcionarios.txt", "r");
    int found = 0;

    Funcionario func = {};
    
    char serialized_func[sizeof(Funcionario)];
    fseek(file, 0, SEEK_SET);
    
    while (fread(&func, sizeof(Funcionario), 1, file))
    {
        sprintf(serialized_func, FUNCIONARIO_FORMAT_IN, func.nome, func.cpf, func.senha);
        if (strcmp(serialized_func, buffer) == 0)
        {
            found = 1;
            break;
        }        
    }

    fclose(file);

    pthread_mutex_unlock(mutex);
    
    // End Critical Session

    return found;
}

void write_user(char* buffer, pthread_mutex_t* mutex)
{
    // Critical Session
    pthread_mutex_lock(mutex);

    FILE* file = fopen("users.txt", "a");
    fprintf(file, "%s\n", buffer);
    fclose(file);

    pthread_mutex_unlock(mutex);
    
    // End Critical Session
}
