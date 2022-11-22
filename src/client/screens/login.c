#include <stdio.h>
#include <stdlib.h>
#include "../headers/client.h"
#include <string.h>
#include "../headers/menu.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../types/funcionario.h"
#include "../../types/request.h"

#define BUFFER_SIZE 132
#define LOGIN_STR_LENGTH 80
#define PASSWORD_STR_LENGTH 50

void fill_with_whitespace (char text[], int array_size);
int log_in(Funcionario* funcionario);

int main(){
	Funcionario funcionario = {};
    int authenticated = 0 ;
    
    while(authenticated == 0){
        clear_screen();
	
        printf("Nome (%d caracteres): ", FUNCIONARIO_NAME_SIZE - 1);
        char name[FUNCIONARIO_NAME_SIZE];
        bzero(name, FUNCIONARIO_NAME_SIZE);
        scanf("%[^\n]%*c", name);
        sprintf(funcionario.nome, FUNCIONARIO_NAME_FORMAT_IN, name);
        
        printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
        char cpf[FUNCIONARIO_CPF_SIZE];
        bzero(cpf, FUNCIONARIO_CPF_SIZE);
        scanf("%s%*c", cpf);
        sprintf(funcionario.cpf, FUNCIONARIO_CPF_FORMAT_IN, cpf);
        
        printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
        char password[FUNCIONARIO_PASSWORD_SIZE];
        bzero(password, FUNCIONARIO_PASSWORD_SIZE);
        scanf("%s%*c", password);
        sprintf(funcionario.senha, FUNCIONARIO_PASSWORD_FORMAT_IN, password);

	    authenticated = log_in(&funcionario);

        if (authenticated)
            menu();
    }

    return 0;
	
}

// Retorno: 0 - Não-Autenticado. 1 - Autenticado
int log_in(Funcionario* funcionario)
{
    int sock = create_sock_connection();

    Request request = {};

    bzero(&request, sizeof(Request));

    char message[sizeof(Request)];
    bzero(message, sizeof(Request));

    sprintf(message, REQUEST_FORMAT, "AUTH", "funcionarios", funcionario->nome, funcionario->cpf, funcionario->senha);

    send_message(sock, message);

    char buffer = '\0';
    
    while(read_answer(sock, &buffer, 1) > 0);

    close_sock_connection(sock);
    
    // consulta a tabela e armazena os dados
    //verifica quantos registros foram buscados através de um loop contando quantos blocos de caracteres foram lidos
    //aloca espaço em memória de acordo com a quantidade de registros contabilizados (qtd * sizeof(Funcionario))
    //lê os registros e armazena-os no array de funcionario
    //retorna o array de funcionario

    if (buffer == '0')
        return 0;

    return 1;
}