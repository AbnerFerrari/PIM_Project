#include "../headers/constants.h"
#include "../../types/funcionario.h"
#include "../headers/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

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
        sprintf(local_buffer, "Id: %d\nNome: %s\nCPF: %s\nSenha: %s\n\n\n", func.id, func.nome, func.cpf, func.senha);
        strcat(buffer, local_buffer);
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

    //sscanf(buffer, FUNCIONARIO_FORMAT_OUT, func.nome, func.cpf, func.senha);
    sscanf(buffer, "%s %s %s", func.nome, func.cpf, func.senha);

    fwrite(&func, sizeof(Funcionario), 1, file);
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
    
    // End Critical Session
}

void database_delete(char* table_name, int id)
{
    FILE *file, *temp;
    union data_access obj;
    int i, j, chunk_size, found=0;

    char* file_name;
    get_file_name_with_extension(table_name, &file_name);

    if (strcmp(table_name, "funcionarios") == 0)
    {
        
        file = fopen(file_name, "r");
        temp = fopen("funcionarios_temp.txt", "a");
        chunk_size = sizeof(Funcionario);
    }
    else {
        perror("erro ao excluir");
    }
    
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
            temp = fopen("funcionarios_temp.txt", "r");

            while(fread(&obj, chunk_size, 1, temp)){
                fwrite(&obj, chunk_size, 1, file);
            }
            fclose(file);
            fclose(temp);
    }
    else
        printf("\nNot Found.....\n");



    // int length = strlen(table_name) + strlen(FILE_EXTENSION) + 1;
    // char file_name[length];
    // get_file_name_with_extension(table_name, length, file_name);

    // // Critical Session
    // pthread_mutex_lock(&mutex_funcionarios_file);

    // FILE* file = fopen(file_name, "a");
    // Funcionario func = {};
    
    // sscanf(buffer, FUNCIONARIO_FORMAT_OUT, func.nome, func.cpf, func.senha);

    // fprintf(file, FUNCIONARIO_FORMAT_IN, func.nome, func.cpf, func.senha);
    // fclose(file);

    // pthread_mutex_unlock(&mutex_funcionarios_file);
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