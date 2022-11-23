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

// Funcionario database_get(char[20] nome_tabela, char[20] nome_campo, char[150] valor_campo){
//     return funcionario;
// }

void save(char* nome_tabela, Funcionario* funcionario){
    int sock = create_sock_connection();

    int table_name_size = strlen(nome_tabela);
    int message_length = 5 + table_name_size + 1 + sizeof(Funcionario);
    
    Request request = {};
    char message[sizeof(Request)];

    bzero(&request, sizeof(Request));
    bzero(&message, sizeof(Request));

    sprintf(message, REQUEST_FORMAT, "POST", "funcionarios", funcionario->nome, funcionario->cpf, funcionario->senha);
    
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);
}

// int database_update(char[20] nome_tabela, Funcionario* funcionario){
//     return 1;
// }

int delete(char* table_name, char* key){
    int sock = create_sock_connection();

    int table_name_size = strlen(table_name);
    int message_length = 5 + table_name_size + 1 + sizeof(Funcionario);
    
    Request request = {};
    char message[sizeof(Request)];

    bzero(&request, sizeof(Request));
    bzero(&message, sizeof(Request));

    sprintf(message, REQUEST_FORMAT, "DELETE", "funcionarios", key);
    
    send_message(sock, message);

    char buffer = '0';

    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);   
    
    if (buffer == '0') perror("erro ao excluir");
}