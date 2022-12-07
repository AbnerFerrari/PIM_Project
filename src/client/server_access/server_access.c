#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../types/user.h"
#include "../../types/order.h"
#include "../../types/request.h"
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

void get(char* table_name, int id){
    // cria conexão com servidor
    int sock = create_sock_connection();

    char message[sizeof(Request)];
    bzero(message, sizeof(Request));

    int length;

    if (strcmp(table_name, "funcionarios") == 0)
    {
        length = sizeof(User);
    } else if (strcmp(table_name, "pedidos") == 0)
    {
        length = sizeof(Order);
    }
    else
    {
        perror("nome da tabela inválido");
    }

    sprintf(message, "%s %s %d", "GET", table_name, id);
    send_message(sock, message);

    char buffer[length];
    bzero(buffer, length);

    while(read_answer(sock, &buffer[0], length) > 0);

    if (strcmp(table_name, "funcionarios") == 0)
    {
        User func = {};
        sscanf(buffer, "%d %s %s %s", &func.id, func.nome, func.cpf, func.senha);
        printf(FUNCIONARIO_PRETTY_FORMAT, func.id, func.nome, func.cpf, func.senha);
    } else if (strcmp(table_name, "pedidos") == 0)
    {
        Order order = {};
        sscanf(buffer, "%d %d %[^\n;]%*c %[^\n;]%*c %d %f", &order.id, &order.userId, order.client, order.product, &order.quantity, &order.unit_price);
        printf("Id: %d\nId do vendedor: %d\nCliente: %s\nProduto: %s\nQuantidade: %d\nValor unitario: %.2f\nValor total: %.2f\n\n\n",
         order.id, order.userId, order.client, order.product, order.quantity, order.unit_price,
         order.quantity * order.unit_price);
    }

    close_sock_connection(sock);
}

void list(char* nome_tabela){
    // cria conexão com servidor
    int sock = create_sock_connection();

    char message[sizeof(Request)];
    bzero(message, sizeof(Request));
    
    sprintf(message, REQUEST_FORMAT, "LIST", nome_tabela, "");
    send_message(sock, message);

    int length;

    if (strcmp(nome_tabela, "funcionarios") == 0)
    {
        length = sizeof(User) * 20;
    }
    else if (strcmp(nome_tabela, "pedidos") == 0)
    {
        length = sizeof(Order) * 20;
    }
    else
    {
        perror("nome da tabela inválido");
    }
    
    char buffer[length];
    bzero(buffer, length);

    while(read_answer(sock, &buffer[0], length) > 0);

    printf("%s", buffer);

    close_sock_connection(sock);
}

void save(char* nome_tabela, char* entity){
    int sock = create_sock_connection();

    int table_name_size = strlen(nome_tabela);
    int message_length = 5 + table_name_size + 1 + sizeof(User);
    
    Request request = {};
    char message[sizeof(Request)];

    bzero(&request, sizeof(Request));
    bzero(&message, sizeof(Request));

    sprintf(message, "%s %s %s", "POST", nome_tabela, entity);
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);
}

void edit(char* nome_tabela, char* entity){
    int sock = create_sock_connection();

    Request request = {};
    char message[sizeof(Request)];
    bzero(&message, sizeof(Request));

    sprintf(message, "%s %s %s", "PUT", nome_tabela, entity);
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);    

    close_sock_connection(sock);
    
    if (buffer = '0')
    {
        perror("Falha ao editar funcionario");
    }
}

int delete(char* table_name, int id){
    int sock = create_sock_connection();

    char message[sizeof(Request)];

    bzero(&message, sizeof(Request));

    sprintf(message, "%s %s %d", "DELETE", table_name, id);
    
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);   
    
    if (buffer == '0') perror("erro ao excluir");
}