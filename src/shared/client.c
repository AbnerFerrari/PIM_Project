#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 9001

int create_socket_connection(int* sock, struct sockaddr_in* sock_addr);
void fill_with_whitespace(char login[], int array_size);

void log_in(char login[], char password[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char login_password[130] = {};
    char buffer[1024] = { 0 };
    
    bzero((char*)&serv_addr, sizeof(serv_addr));
    int conn_created = create_socket_connection(&sock, &serv_addr);
    if (conn_created < 0)
    {
        printf("Error while creating socket");
    }
    
    fill_with_whitespace(login, 79);
    strcpy(login_password, login);
    fill_with_whitespace(password, 49);
    strcat(login_password, password);

    send(sock, login_password, strlen(login_password), 0);
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    // closing the connected socket
    int closed = close(0);
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

    int status_conn = connect((*sock), (struct sockaddr*)serv_addr, sizeof((*serv_addr)));
    if(status_conn < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
}