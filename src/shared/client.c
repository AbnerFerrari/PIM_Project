#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 9001

int create_sock_connection()
{
    int sock = 0, valread, conn_status;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço IP do servidor
    
    conn_status = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (conn_status < 0)
    {
        printf("Error while creating socket");
    }

    return sock;
}

void send_message(int sock, char* message){
    send(sock, message, strlen(message), 0);
}

int read_answer(int sock, char* buffer, int buffer_size){
    int status = read(sock, buffer, buffer_size);
    return status;
}

void close_sock_connection(int sock){
    close(sock);
}