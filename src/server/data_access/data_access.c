#include "../headers/constants.h"
#include "../../types/funcionario.h"
#include "../headers/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

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
        sprintf(local_buffer, FUNCIONARIO_PRETTY_FORMAT, func.nome, func.cpf, func.senha);
        strcat(buffer, local_buffer);
    }
    
    fclose(file);

    pthread_mutex_unlock(mutex);
    // End Critical Session
}

int database_write(char* table_name, char* buffer, pthread_mutex_t* mutex)
{
    printf("%s", buffer);
    int length = strlen(table_name) + strlen(FILE_EXTENSION);
    char file_name[length];
    get_file_name_with_extension(table_name, length, file_name);

    // Critical Session
    pthread_mutex_lock(mutex);

    FILE* file = fopen(file_name, "a");
    Funcionario func = {};
    
    sscanf(buffer, FUNCIONARIO_FORMAT_OUT, func.nome, func.cpf, func.senha);
    
    // func.nome[FUNCIONARIO_NAME_SIZE - 1] = '\0';
    // func.cpf[FUNCIONARIO_CPF_SIZE - 1] = '\0';
    // func.senha[FUNCIONARIO_PASSWORD_SIZE - 1] = '\0';

    fprintf(file, FUNCIONARIO_FORMAT_IN, func.nome, func.cpf, func.senha);
    fclose(file);

    pthread_mutex_unlock(mutex);
    
    // End Critical Session
}

int user_exists(char* buffer, pthread_mutex_t* mutex)
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
        if (strncmp(serialized_func, buffer, 84) == 0)
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