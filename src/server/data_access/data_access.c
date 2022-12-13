#include "../headers/constants.h"
#include "../../types/user.h"
#include "../../types/order.h"
#include "../../types/order_by_user.h"
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
    char local_buffer[chunk_size + 94];

    FILE* file = fopen("pedidos.txt", "r");

    while(fread(&current_order, chunk_size, 1, file) != 0){
        bzero(local_buffer, chunk_size);
        sprintf(local_buffer, "Id: %d\nId do vendedor: %d\nCliente: %s\nProduto: %s\nQuantidade: %d\nValor unitario: %.2f\nValor total: %.2f\n\n\n",
         current_order.id, current_order.userId, current_order.client, current_order.product, current_order.quantity, current_order.unit_price,
         current_order.quantity * current_order.unit_price);
        strcat(buffer, local_buffer);
    }

    fclose(file);

    pthread_mutex_unlock(&mutex_pedidos_file);
}

void database_read(char* table_name, char* buffer){
    if (strcmp(table_name, "funcionarios") == 0)
    {
        read_users(buffer);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        read_orders(buffer);
    }
}

void get_user(int id, char** entity){
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen("funcionarios.txt", "r");

    User func = {};
    *entity = malloc(sizeof(User));
    while(fread(&func, sizeof(User), 1, file) != 0){
        if (func.id == id)
        {
            sprintf(*entity, "%d %s; %s %s", func.id, func.nome, func.cpf, func.senha);
            break;
        }
    }
    
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void get_order(int id, char** entity){
    pthread_mutex_lock(&mutex_pedidos_file);

    FILE* file = fopen("pedidos.txt", "r");

    Order order = {};
    *entity = malloc(sizeof(Order));
    while(fread(&order, sizeof(Order), 1, file) != 0){
        if (order.id == id)
        {
            sprintf(*entity, "%d %d %s; %s; %d %f", order.id, order.userId, order.client, order.product, order.quantity, order.unit_price);
            break;
        }
    }
    
    fclose(file);

    pthread_mutex_unlock(&mutex_pedidos_file);
}

void database_get(char* table_name, int id, char** entity){
    if (strcmp(table_name, "funcionarios") == 0)
    {
        get_user(id, entity);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        get_order(id, entity);
    }
}

int get_order_by_user_index(OrderByUser* order_by_user, int user_id){
    char* entity;
    User user = {};
    database_get("funcionarios", user_id, &entity);
    sscanf(entity, "%d %[^\n;]%*c %s %s", &user.id, user.nome, user.cpf, user.senha);
    
    for (size_t i = 0; i < 20; i++)
    {
        if (strcmp(order_by_user[i].user, user.nome) == 0)
            return i;
    }

    return -1;
}

void get_report_data(char* report_name, char** buffer){
    if (strcmp(report_name, "report_orders_by_user") == 0)
    {
        pthread_mutex_lock(&mutex_pedidos_file);

        FILE* pedidos_file = fopen("pedidos.txt", "r");
        
        *buffer = malloc(sizeof(OrderByUser) * 30);
        bzero(*buffer, sizeof(OrderByUser) * 30);
        Order order = { };
        User user = { };
        OrderByUser order_by_user[20];
        int i = 0;
        while (fread(&order, sizeof(Order), 1, pedidos_file) != 0)
        {
            // se existir, retorna o index de order_by_user, caso contrario retorna -1
            int index = get_order_by_user_index(order_by_user, order.userId);
            if (index < 0)
            {
                order_by_user[i].quantity_orders = 1;
                order_by_user[i].total_orders_price = order.unit_price * order.quantity;

                pthread_mutex_lock(&mutex_funcionarios_file);
                FILE* funcionarios_file = fopen("funcionarios.txt", "r");

                while (fread(&user, sizeof(User), 1, funcionarios_file) != 0)
                {
                    if (order.userId == user.id)
                    {
                        strcpy(order_by_user[i].user, user.nome);
                        break;
                    }
                }
                fclose(funcionarios_file);
                pthread_mutex_unlock(&mutex_funcionarios_file);

                i++;
            } else {
                order_by_user[index].quantity_orders++;
                order_by_user[index].total_orders_price += order.unit_price * order.quantity;
            }
        }
        
        fclose(pedidos_file);
        pthread_mutex_unlock(&mutex_pedidos_file);

        char local_buffer[sizeof(OrderByUser) + 60];
        for (size_t j = 0; j < i; j++)
        {
            bzero(local_buffer, sizeof(OrderByUser) + 60);
            sprintf(local_buffer, ORDER_BY_USER_PRETTY_FORMAT, order_by_user[j].user, order_by_user[j].quantity_orders, order_by_user[j].total_orders_price);
            strcat(*buffer, local_buffer);
        }   
    }
}

void encrypt(char* password){
    for (size_t i = 0; i < strlen(password); i++)
    {        
        password[i] = (int)password[i] + 1;
    }
}

void write_user(char* buffer){
    pthread_mutex_lock(&mutex_funcionarios_file);

    User func = {};
    User last_func = {};

    #pragma region GetLastId
    FILE* file = fopen("funcionarios.txt", "r");
    while (fread(&last_func, sizeof(User), 1, file) != 0);
    func.id = last_func.id + 1;
    fclose(file);
    #pragma endregion

    file = fopen("funcionarios.txt", "a");
    sscanf(buffer, "%[^\n;]%*c %s %s", func.nome, func.cpf, func.senha);

    encrypt(func.senha);

    fwrite(&func, sizeof(User), 1, file);
    fclose(file);

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void write_order(char* buffer){
    // Critical Session
    pthread_mutex_lock(&mutex_pedidos_file);

    Order order = {};
    Order last_order = {};

    #pragma region GetLastId

    FILE* file = fopen("pedidos.txt", "a");
    fclose(file);
    file = fopen("pedidos.txt", "r");
    
    while (fread(&last_order, sizeof(Order), 1, file) != 0);
    order.id = last_order.id + 1;
    fclose(file);
    #pragma endregion

    file = fopen("pedidos.txt", "a");

    sscanf(buffer, "%d %[^\n;]%*c %[^\n;]%*c %d %f", &order.userId, order.client, order.product, &order.quantity, &order.unit_price);

    fwrite(&order, sizeof(Order), 1, file);
    fclose(file);

    pthread_mutex_unlock(&mutex_pedidos_file);
}

int database_write(char* table_name, char* buffer)
{
    if (strcmp(table_name, "funcionarios") == 0)
    {
        write_user(buffer);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        write_order(buffer);
    }
}

void delete_user(int id){
    pthread_mutex_lock(&mutex_funcionarios_file);
    FILE *file, *temp;
    User user = { };
    int i, j, chunk_size, found=0;

    char* file_name = "funcionarios.txt";
    char* table_name = "funcionarios";
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);

    file = fopen(file_name, "r");
    chunk_size = sizeof(User);

    temp = fopen(temp_file_name, "w");
    while(fread(&user, chunk_size, 1, file)){
        if(user.id == id){
            found = 1;
        }
        else
            fwrite(&user, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&user, chunk_size, 1, temp)){
            fwrite(&user, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void delete_order(int id){
    pthread_mutex_lock(&mutex_pedidos_file);
    FILE *file, *temp;
    Order order = { };
    int i, j, chunk_size, found=0;

    char* file_name = "pedidos.txt";
    char* table_name = "pedidos";
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);

    file = fopen(file_name, "r");
    chunk_size = sizeof(Order);

    temp = fopen(temp_file_name, "w");
    while(fread(&order, chunk_size, 1, file)){
        if(order.id == id){
            found = 1;
        }
        else
            fwrite(&order, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&order, chunk_size, 1, temp)){
            fwrite(&order, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }

    pthread_mutex_unlock(&mutex_pedidos_file);
}

void database_delete(char* table_name, int id)
{
    if (strcmp(table_name, "funcionarios") == 0)
    {
        delete_user(id);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        delete_order(id);
    }  
}

void update_user(char* buffer){
    pthread_mutex_lock(&mutex_funcionarios_file);
    FILE *file, *temp;
    User user;
    int i, j, chunk_size, found=0;

    char* file_name = "funcionarios.txt";
    char* table_name = "funcionarios";
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);

    User func = {};
    
    file = fopen(file_name, "r");
    chunk_size = sizeof(User);
    sscanf(buffer, "%d %[^\n;]%*c %s %s", &func.id, func.nome, func.cpf, func.senha);

    encrypt(func.senha);

    temp = fopen(temp_file_name, "w");
    while(fread(&user, chunk_size, 1, file)){
        if(user.id == func.id){
            found = 1;
            fwrite(&func, chunk_size, 1, temp);
        }
        else
            fwrite(&user, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&user, chunk_size, 1, temp)){
            fwrite(&user, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }
    else
        printf("\nNot Found.....\n");

    pthread_mutex_unlock(&mutex_funcionarios_file);
}

void update_order(char* buffer){
    pthread_mutex_lock(&mutex_pedidos_file);
    Order order = {};
    FILE *file, *temp;
    int i, j, chunk_size, found=0;

    char* file_name = "pedidos.txt";
    char* table_name = "pedidos";
    
    char timestamp[11];
    sprintf(timestamp, "%d", time(NULL));
    char* temp_file_name = malloc(strlen(table_name) + strlen(FILE_EXTENSION) + strlen(timestamp));
    sprintf(temp_file_name, "%s%s%s", table_name, timestamp, FILE_EXTENSION);

    file = fopen(file_name, "r");
    chunk_size = sizeof(Order);
    sscanf(buffer, "%d %d %[^\n;]%*c %[^\n;]%*c %d %f", &order.id, &order.userId, order.client, order.product, &order.quantity, &order.unit_price);

    temp = fopen(temp_file_name, "w");
    Order temp_order = {};
    while(fread(&temp_order, chunk_size, 1, file)){
        if(temp_order.id == order.id){
            found = 1;
            fwrite(&order, chunk_size, 1, temp);
        }
        else
            fwrite(&temp_order, chunk_size, 1, temp);
    }
    fclose(file);
    fclose(temp);


    if(found){
        file = fopen(file_name, "w");
        temp = fopen(temp_file_name, "r");

        while(fread(&temp_order, chunk_size, 1, temp)){
            fwrite(&temp_order, chunk_size, 1, file);
        }
        fclose(file);
        fclose(temp);
        remove(temp_file_name);
    }
    else
        printf("\nNot Found.....\n");

    pthread_mutex_unlock(&mutex_pedidos_file);
}

void database_update(char* table_name, char* buffer){
    if (strcmp(table_name, "funcionarios") == 0)
    {
        update_user(buffer);
    }
    else if (strcmp(table_name, "pedidos") == 0)
    {
        update_order(buffer);
    }
}

int user_exists(char* buffer)
{
    // Critical Session
    pthread_mutex_lock(&mutex_funcionarios_file);

    FILE* file = fopen("funcionarios.txt", "r");
    int found = 0;

    User func = {};
    User login = {};
    sscanf(buffer, "%[^\n;]%*c %s %s", &login.nome, login.cpf, login.senha);
    encrypt(login.senha);
    char serialized_func[sizeof(User)];
    fseek(file, 0, SEEK_SET);
    
    while (fread(&func, sizeof(User), 1, file))
    {
        if (strcmp(func.nome, login.nome) == 0
            && strcmp(func.cpf, login.cpf) == 0
            && strcmp(func.senha, login.senha) == 0)
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