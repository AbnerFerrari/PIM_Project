#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 9001
#define BUFFER_SIZE 132

int create_socket_connection(int* sock, struct sockaddr_in* sock_addr);
void fill_with_whitespace(char login[], int array_size);

void log_in(char login[], char password[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char login_password[130] = {};
    char buffer[BUFFER_SIZE] = { 0 };
    
    bzero((char*)&serv_addr, sizeof(serv_addr));
    client_fd = create_socket_connection(&sock, &serv_addr);
    if (client_fd < 0)
    {
        printf("Error while creating socket");
    }
    
    fill_with_whitespace(login, 81);
    strcpy(login_password, login);
    fill_with_whitespace(password, 51);
    strcat(login_password, password);

    send(sock, login_password, strlen(login_password), 0);
    printf("%s\n", buffer);
    
    read(sock, buffer, BUFFER_SIZE);
    printf("%s", buffer);
    
    // closing the connected socket
    int closed = close(client_fd);
}

void fill_with_whitespace (char text[], int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        if (text[i] == '\0')
        {
            for (int j = i; j < array_size; j++)
            {
                text[j] = ' ';
            }

            break;
        }
    }

    text[array_size] = '\0';
}

int create_socket_connection(int* sock, struct sockaddr_in* serv_addr)
{
    int client_fd;
    if (((*sock) = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);
    serv_addr->sin_addr.s_addr = inet_addr("127.0.0.1"); // Endereço IP do servidor

    client_fd = connect((*sock), (struct sockaddr*)serv_addr, sizeof((*serv_addr)));
    if(client_fd < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return client_fd;
}