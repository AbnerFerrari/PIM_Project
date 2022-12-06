#include "../headers/constants.h"
#include "../../types/user.h"
#include "../../types/order.h"
#include "../headers/utils.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union data_access
{
    int id;
    User func;
    Order order;
} Data;


pthread_mutex_t mutex_funcionarios_file = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_pedidos_file = PTHREAD_MUTEX_INITIALIZER;

void read_users(char* buffer){
    int chunk_size = sizeof(User);
    pthread_mutex_lock(&mutex_funcionarios_file);

    User current_user = {};
    char local_buffer[chunk_size];

    FILE* file = fopen("funcionarios.txt", "r");

    while(fread(&current_user, chunk_size, 1, file) != 0){
        bzero(local_buffer, chunk_size);
        sprintf(local_buffer, FUNCIONARIO_PRETTY_FORMAT, current_user.id, current_user.nome, current_user.cpf, current_user.senha);
        strcat(buffer, local_buffer);
    }

    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void read_orders(char* buffer){
    int chunk_size = sizeof(Order);
    pthread_mutex_lock(&mutex_pedidos_file);

    Order current_order = {};
    char local_buffer[chunk_size];

    FILE* file = fopen("pedidos.txt", "r");

    while(fread(&current_order, chunk_size, 1, file) != 0){
        bzero(local_buffer, chunk_size);
        sprintf(local_buffer, FUNCIONARIO_PRETTY_FORMAT, current_order.id, current_order.userId, current_order.client,
                current_order.product, current_order.quantity, current_order.unit_price);
        strcat(buffer, local_buffer);
    }

    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void database_read(char* table_name, char* buffer, long buffer_size){
    if (strcmp(table_name, "funcionarios") == 0)
    {
        read_users(buffer);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        read_orders(buffer);
    }
}

void database_get(char* table_name, int id, char** entity){
    char* file_name;
    get_file_name_with_extension(table_name, &file_name);

    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen(file_name, "r");

    User func = {};
    *entity = malloc(sizeof(User));
    while(fread(&func, sizeof(User), 1, file) != 0){
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

    User func = {};
    User last_func = {};

    #pragma region GetLastId
    FILE* file = fopen(file_name, "r");
    while (fread(&last_func, sizeof(User), 1, file) != 0);
    func.id = last_func.id + 1;
    fclose(file);
    #pragma endregion

    file = fopen(file_name, "a");

    sscanf(buffer, "%s %s %s", func.nome, func.cpf, func.senha);

    fwrite(&func, sizeof(User), 1, file);
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
        chunk_size = sizeof(User);
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


    User func = {};
    if (strcmp(table_name, "funcionarios") == 0)
    {
        file = fopen(file_name, "r");
        chunk_size = sizeof(User);
        
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

    User func = {};
    
    char serialized_func[sizeof(User)];
    fseek(file, 0, SEEK_SET);
    
    while (fread(&func, sizeof(User), 1, file))
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