#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "../types/funcionario.h"
#include "./headers/data_access.h"
#include "./headers/utils.h"
#include "./headers/constants.h"
#include "../types/request.h"

void init_server(int* server_fd, struct sockaddr_in* address, int addrlen);
void* read_message(void* arg);

pthread_mutex_t mutex_thread_counter = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 0, empty_slot_index = 0;
pthread_t threads[NUM_THREADS] = {};

void main(int argc, char const* argv[]){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[sizeof(Request)] = { 0 };
    Request infos = {};

    init_server(&server_fd, &address, addrlen);

    while (1) // Loop infinito
    {
        if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (thread_count == NUM_THREADS); // Fica preso no loop enquanto não houver threads disponiveis
        
        pthread_mutex_lock(&mutex_thread_counter);
        thread_count++;
        pthread_mutex_unlock(&mutex_thread_counter);

        for (size_t i = 0; i < NUM_THREADS; i++)
        {
            if (threads[i] == 0)
            {
                empty_slot_index = i;
                break;
            }
        }
        
        
        bzero(buffer, sizeof(Request));
        bzero(&infos, sizeof(infos));
        read(new_socket, buffer, 240);

        sscanf(buffer, "%s %s %[^\n]s", infos.action, infos.table, infos.body);

        infos.sock = new_socket;
        infos.running_thread_index = empty_slot_index;


        // fazer isso com todas as propriedades
        int file_name_size = strlen(strtok(infos.table, " "));
        char file_name[file_name_size];
        strcpy(file_name, strtok(infos.table, " "));
        bzero(infos.table, sizeof(infos.table));
        strcpy(infos.table, file_name);

        pthread_create(&threads[empty_slot_index], NULL, read_message, (void*)&infos);
    }
    
    // Closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    pthread_exit(NULL);
}

void* read_message(void* arg)
{
    pthread_t t_id = pthread_self();
    printf("Running Thread: %ld\n", t_id);
    
    struct request infos = *(struct request*)arg;

    if (strncmp(infos.action, "LIST", 4) == 0)
    {
        long file_size = get_file_size(infos.table);
        
        long size_with_format;
        int chunk_size;
        int quantity_metadata_chars;
        
        if (strcmp(infos.table, "funcionarios") == 0)
        {
            quantity_metadata_chars = 23;
            size_with_format = (file_size / sizeof(Funcionario)) * quantity_metadata_chars + file_size;
            chunk_size = sizeof(Funcionario);
        }
        else
        {
            perror("nome da tabela é invalido");
        }
        
        
        char list_buffer[size_with_format];
        database_read(infos.table, list_buffer, file_size, chunk_size, quantity_metadata_chars);
        
        send(infos.sock, list_buffer, size_with_format, 0);
        // lê a tabela que a ação deve ser executada
        // lê o arquivo inteiro
    }
    else if (strncmp(infos.action, "GET", 3) == 0)
    {
        char* entity;
        database_get(infos.table, atoi(infos.body), &entity);
        send(infos.sock, entity, strlen(entity), 0);
    }
    else if (strncmp(infos.action, "POST", 4) == 0)
    {
        database_write(infos.table, &infos.body[0]);
    }
    else if (strncmp(infos.action, "PUT", 3) == 0)
    {
        database_update(infos.table, &infos.body[0]);
    }
    else if (strncmp(infos.action, "DELETE", 6) == 0 )
    {
        database_delete(infos.table, atoi(infos.body));
    }
    else // rotina de login. Isolar esse código
    {
        int exists = user_exists(infos.body);
        
        if (exists)
        {
            char message = '1';
            send(infos.sock, &message, sizeof(message), 0);
        }
        else
        {
            char message = '0';
            send(infos.sock, &message, sizeof(message), 0);
        }
    }

    // Closing the connected socket
    close(infos.sock);

    pthread_mutex_lock(&mutex_thread_counter);
    thread_count--;
    threads[infos.running_thread_index] = 0;
    pthread_mutex_unlock(&mutex_thread_counter);
}

void init_server(int* server_fd, struct sockaddr_in* address, int addrlen){
    int opt = 1;
    char buffer[1024] = { 0 };

    //Creating socket file descriptor
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (*server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if(setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if(bind(*server_fd, (struct sockaddr*)address, addrlen) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(*server_fd, NUM_THREADS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}