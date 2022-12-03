#include "../headers/constants.h"
#include "../../types/user.h"
#include "../headers/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union data_access
{
    int id;
    Funcionario func;
} Data;


pthread_mutex_t mutex_funcionarios_file = PTHREAD_MUTEX_INITIALIZER;

void database_read(char* table_name, char* buffer, long buffer_size, int chunk_size, int metadata_size){
    char* file_name;
    get_file_name_with_extension(table_name, &file_name);

    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen(file_name, "r");
    
    fseek(file, 0, SEEK_END); 
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 

    Funcionario func = {};
    int times = buffer_size / chunk_size;
    char local_buffer[chunk_size + metadata_size];

    while(fread(&func, chunk_size, 1, file) != 0){
        bzero(local_buffer, chunk_size);
        sprintf(local_buffer, FUNCIONARIO_PRETTY_FORMAT, func.id, func.nome, func.cpf, func.senha);
        strcat(buffer, local_buffer);
    }
    
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
    // End Critical Session
}

void database_get(char* table_name, int id, char** entity){
    char* file_name;
    get_file_name_with_extension(table_name, &file_name);

    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen(file_name, "r");

    Funcionario func = {};
    *entity = malloc(sizeof(Funcionario));
    while(fread(&func, sizeof(Funcionario), 1, file) != 0){
        if (func.id == id)
        {
            sprintf(*entity, "%d %s %s %s", func.id, func.nome, func.cpf, func.senha);
            break;
        }
    }
    
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
    // End Critical Session
}

int database_write(char* table_name, char* buffer)
{
    char* file_name;
    get_file_name_with_extension(table_name, &file_name);

    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    Funcionario func = {};
    Funcionario last_func = {};

    #pragma region GetLastId
    FILE* file = fopen(file_name, "r");
    while (fread(&last_func, sizeof(Funcionario), 1, file) != 0);
    func.id = last_func.id + 1;
    fclose(file);
    #pragma endregion

    file = fopen(file_name, "a");

    sscanf(buffer, "%s %s %s", func.nome, func.cpf, func.senha);

    fwrite(&func, sizeof(Funcionario), 1, file);
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
    
    // End Critical Session
}

void database_delete(char* table_name, int id)
{
    pthread_mutex_lock(&mutex_funcionarios_file);
    FILE *file, *temp;
    union data_access obj;
    int i, j, chunk_size, found=0;

    char* file_name;
    get_file_name_with_extension(table_name, &file_name);
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);

    if (strcmp(table_name, "funcionarios") == 0)
    {
        file = fopen(file_name, "r");
        chunk_size = sizeof(Funcionario);
    }
    else {
        perror("erro ao excluir");
    }

    temp = fopen(temp_file_name, "w");
    while(fread(&obj, chunk_size, 1, file)){
        if(obj.id == id){
            found = 1;
        }
        else
            fwrite(&obj, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&obj, chunk_size, 1, temp)){
            fwrite(&obj, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void database_update(char* table_name, char* buffer){
    pthread_mutex_lock(&mutex_funcionarios_file);
    FILE *file, *temp;
    union data_access obj;
    int i, j, chunk_size, found=0;

    char* file_name;
    get_file_name_with_extension(table_name, &file_name);
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);


    Funcionario func = {};
    if (strcmp(table_name, "funcionarios") == 0)
    {
        file = fopen(file_name, "r");
        chunk_size = sizeof(Funcionario);
        
        sscanf(buffer, "%d %s %s %s", &func.id, func.nome, func.cpf, func.senha);
    }
    else {
        perror("erro ao excluir");
    }

    temp = fopen(temp_file_name, "w");
    while(fread(&obj, chunk_size, 1, file)){
        if(obj.id == func.id){
            found = 1;
            fwrite(&func, chunk_size, 1, temp);
        }
        else
            fwrite(&obj, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&obj, chunk_size, 1, temp)){
            fwrite(&obj, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }
    else
        printf("\nNot Found.....\n");

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

int user_exists(char* buffer)
{
    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen("funcionarios.txt", "r");
    int found = 0;

    Funcionario func = {};
    
    char serialized_func[sizeof(Funcionario)];
    fseek(file, 0, SEEK_SET);
    
    while (fread(&func, sizeof(Funcionario), 1, file))
    {
        sprintf(serialized_func, "%s %s %s", func.nome, func.cpf, func.senha);
        if (strncmp(serialized_func, buffer, 88) == 0)
        {
            found = 1;
            break;
        }        
    }

    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
    
    // End Critical Session

    return found;
}

long get_file_size(char* table_name)
{ 
    pthread_mutex_lock(&mutex_funcionarios_file);
    char* file_name;
    get_file_name_with_extension(table_name, &file_name);
    
    FILE* file = fopen(file_name, "r");
    
    fseek(file, 0, SEEK_END); 
    long size = ftell(file);
    fseek(file, 0, SEEK_SET); 

    fclose(file);
    pthread_mutex_unlock(&mutex_funcionarios_file);

    return size;
}