#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../types/funcionario.h"
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
        length = sizeof(Funcionario);
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
        Funcionario func = {};
        sscanf(buffer, "%d %s %s %s", &func.id, func.nome, func.cpf, func.senha);
        printf(FUNCIONARIO_PRETTY_FORMAT_WITH_ID, func.id, func.nome, func.cpf, func.senha);
    }

    close_sock_connection(sock);
}

void list(char* nome_tabela){
    // cria conexão com servidor
    int sock = create_sock_connection();

    char message[sizeof(Request)];
    bzero(message, sizeof(Request));
    
    sprintf(message, REQUEST_FORMAT, "LIST", "funcionarios", "");
    send_message(sock, message);

    int length;

    if (strcmp(nome_tabela, "funcionarios") == 0)
    {
        length = sizeof(Funcionario) * 20;
    }
    else
    {
        perror("nome da tabela inválido");
    }
    
    char buffer[length];
    bzero(buffer, length);

    while(read_answer(sock, &buffer[0], length) > 0);

    printf("%s", buffer);
    
    //Funcionario funcionarios[1] = sscanf("");
    // consulta a tabela e armazena os dados
    //verifica quantos registros foram buscados através de um loop contando quantos blocos de caracteres foram lidos
    //aloca espaço em memória de acordo com a quantidade de registros contabilizados (qtd * sizeof(Funcionario))
    //lê os registros e armazena-os no array de funcionario
    //retorna o array de funcionario

    close_sock_connection(sock);
}

void save(char* nome_tabela, char* entity){
    int sock = create_sock_connection();

    int table_name_size = strlen(nome_tabela);
    int message_length = 5 + table_name_size + 1 + sizeof(Funcionario);
    
    Request request = {};
    char message[sizeof(Request)];

    bzero(&request, sizeof(Request));
    bzero(&message, sizeof(Request));

    sprintf(message, "%s %s %s", "POST", "funcionarios", entity);
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);
}

void edit(char* nome_tabela, char* entity){
    int sock = create_sock_connection();

    int table_name_size = strlen(nome_tabela);
    int message_length = 5 + table_name_size + 1 + sizeof(Funcionario);
    
    Request request = {};
    char message[sizeof(Request)];

    bzero(&request, sizeof(Request));
    bzero(&message, sizeof(Request));

    sprintf(message, "%s %s %s", "PUT", "funcionarios", entity);
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

    sprintf(message, "%s %s %d", "DELETE", "funcionarios", id);
    
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);   
    
    if (buffer == '0') perror("erro ao excluir");
}