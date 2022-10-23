#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 9001
#define NUM_THREADS 2
#define BUFFER_SIZE 1024

struct read_message_args{
    int* sock;
    int* thread_index;
};
void init_server(int* server_fd, struct sockaddr_in* address, int addrlen);
void* read_message(void* arg);
void thread_control(int* thread_index);
pthread_t threads[NUM_THREADS] = { };

void main(int argc, char const* argv[]){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };
    int thread_index = 0;

    init_server(&server_fd, &address, addrlen);
    pthread_t t_id = pthread_self();
    printf("Listening Thread: %ld\n", t_id);

    while (1)
    {
        if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        struct read_message_args args = { &new_socket, &thread_index };
        
        while (thread_index == -1); // Fica preso no loop enquanto não houver threads disponiveis

        pthread_create(&threads[thread_index], NULL, read_message, (void*)&args);
        
        thread_control(&thread_index);
    }
    
    // Closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    pthread_exit(NULL);
}

void* read_message(void* arg){
    pthread_t t_id = pthread_self();
    printf("Running Thread: %ld\n", t_id);
    struct read_message_args* args = (struct read_message_args*)arg;

    int* socket = args->sock;
    int* thread_index = args->thread_index;
    char buffer[BUFFER_SIZE];
    read((*socket), buffer, BUFFER_SIZE);
    printf("Login e senha recebidos: \n");
    printf("%s\n", buffer);
    char* message = "Login realizado com sucesso!";
    send((*socket), message, strlen(message), 0);
    
    // Closing the connected socket
    close((*socket));

    int index = (*thread_index);
    threads[index] = 0; // livrando espaço no array de threads
}

/* Determina se existe slots livres para se criar threads. Se sim, atribui o index do array de threads com espaço livre.
 Caso não possua espaço livre, valor do index é -1 */
void thread_control(int* thread_index){
    
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        if (threads[i] == 0) // Não tenho certeza se o ID da thread pode o. Possivel bug
        {
            *thread_index = i;
            break;
        }

        *thread_index = -1;
    }
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