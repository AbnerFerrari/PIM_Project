#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 9001
#define NUM_THREADS 2
#define BUFFER_SIZE 132

struct info{
    int sock;
    int running_thread_index;
};

void init_server(int* server_fd, struct sockaddr_in* address, int addrlen);
void* read_message(void* arg);
void thread_control(int* thread_index);
int user_exists();

pthread_mutex_t mutex_thread_counter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_users_file = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 0, empty_slot = 0;
pthread_t threads[NUM_THREADS] = {};

void main(int argc, char const* argv[]){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };

    init_server(&server_fd, &address, addrlen);

    while (1) // Loop infinito
    {
        // TODO: precisa criar um array de sockets se não uma thread pode influenciar no socket da outra
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
                empty_slot = i;
                break;
            }
        }
        
        struct info infos = { new_socket, empty_slot };
        pthread_create(&threads[empty_slot], NULL, read_message, (void*)&infos);
    }
    
    // Closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    pthread_exit(NULL);
}

void* read_message(void* arg)
{
    pthread_t t_id = pthread_self();
    printf("Running Thread: %ld\n", t_id);
    struct info infos = *(struct info*)arg;
    char buffer[BUFFER_SIZE];
    read(infos.sock, buffer, BUFFER_SIZE);
    
    int exists = user_exists(buffer);

    if (exists)
    {
        char* message = "Login realizado com sucesso!";
        send(infos.sock, message, strlen(message), 0);
    }
    else
    {
        char* message = "Usuário não cadastrado!";
        send(infos.sock, message, strlen(message), 0);
    }
    
    // Closing the connected socket
    close(infos.sock);

    pthread_mutex_lock(&mutex_thread_counter);
    thread_count--;
    threads[infos.running_thread_index] = 0;
    pthread_mutex_unlock(&mutex_thread_counter);
}

// Retornos possíves: 0 - Não existe. 1 - Existe
int user_exists(char buffer[BUFFER_SIZE])
{
    char original_buffer[BUFFER_SIZE];
    memcpy(original_buffer, buffer, BUFFER_SIZE);

    // Critical Session
    pthread_mutex_lock(&mutex_users_file);

    FILE* file = fopen("users.txt", "r");
    int found = 0;
    while (fgets(buffer, BUFFER_SIZE+1, file) != NULL)
    {
        if (strcmp(original_buffer, buffer) == 0)
        {
            found = 1;
            break;
        }
    }
    
    fclose(file);

    pthread_mutex_unlock(&mutex_users_file);
    
    // End Critical Session

    return found;
}

void write_user(char* buffer)
{
    // Critical Session
    pthread_mutex_lock(&mutex_users_file);

    FILE* file = fopen("users.txt", "a");
    fprintf(file, "%s\n", buffer);
    fclose(file);

    pthread_mutex_unlock(&mutex_users_file);
    
    // End Critical Session
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
    if(bind(*server_fd, (struct sockaddr*)*&address, addrlen) < 0)
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