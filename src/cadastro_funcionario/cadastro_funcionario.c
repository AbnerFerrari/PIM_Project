#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "../shared/client.h"
#include "../types/funcionario.h"


char* list(char* nome_tabela);
// Funcionario database_get(char[20] nome_tabela, char[20] nome_campo, char[150] valor_campo);
// int database_create(char[20] nome_tabela, Funcionario* funcionario);
// int database_update(char[20] nome_tabela, Funcionario* funcionario);
// int database_delete(char[20] nome_tabela, Funcionario* funcionario);

void cadastro_funcionario(int sock){
    int option;
    

    do
    {
        system("clear");
        printf("1 - Listar\n2 - Cadastrar novo funcionário\n3 - Editar funcionário\n4 - Excluir funcionário\n5 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        switch (option)
        {
            case 1: // listar funcionários
                system("clear");
                char* funcionarios_list = list("funcionarios"); // nome da tabela
                printf("Pressione ENTER para retornar");
                char x;
                scanf("%c%*c", &x);
                
                break;
            case 2:
                // criar novo funcionário
                // Funcionario funcionario = { "Adalberto", "12345678910", "123456"};
                // database_create("funcionarios", &funcionario); // nome da tabela, tupla a ser salva

                break;
            case 3:
                // editar funcionário
                // printf("Informe o nome do funcionário a ser editado: ");
                
                // Funcionario funcionario;
                // scanf("%s", funcionario.name);
                
                // funcionario = database_get("funcionarios", "name", funcionario.name); // nome da tabela, nome do campo, valor do campo

                // imprimir_funcionario(&funcionario);
                // printf("Escolha o campo que deseja editar: name, cpf, password");
                
                // scanf("%s", nome_campo);
                // printf("Informe o novo valor para o campo %s:", nome_campo);
                
                // switch (nome_campo)
                // {
                //     case "name":
                //         scanf("%s", funcionario.name);
                //         break;
                //     case "cpf":
                //         scanf("%s", funcionario.cpf);
                //         break;
                //     case "password":
                //         scanf("%s", funcionario.password);
                //         break;
                //     default:
                //         break;
                // }

                // database_update("funcionarios", funcionario); // nome da tabela, funcionario à ser atualizado

                break;
            case 4:
                // printf("Informe o nome do funcionário a ser ecluido: ");
                
                // Funcionario funcionario;
                // scanf("%s", funcionario.name);

                // funcionario = database_get("funcionarios", "name", funcionario.name);

                // if (funcionario == NULL)
                //     printf("Funcionário não encontrado.");
                // else
                //     database_delete("funcionarios", &funcionario);

                break;
            case 5:
                option = 0;
                break;
            
            default:
                break;
        }
    } while (option != 0);
}

char* list(char* nome_tabela){
    // cria conexão com servidor
    int sock = create_sock_connection();
                    //ação tabela nomecampo valorcampo
    char* message = "LIST funcionarios";

    send_message(sock, message);

    int length = sizeof(Funcionario) * 20;
    char buffer[length];
    bzero(buffer, length);

    while(read_answer(sock, buffer, length) > 0);

    printf("%s", buffer);
    
    //Funcionario funcionarios[1] = sscanf("");
    // consulta a tabela e armazena os dados
    //verifica quantos registros foram buscados através de um loop contando quantos blocos de caracteres foram lidos
    //aloca espaço em memória de acordo com a quantidade de registros contabilizados (qtd * sizeof(Funcionario))
    //lê os registros e armazena-os no array de funcionario
    //retorna o array de funcionario

    close_sock_connection(sock);

    return buffer;
}

// Funcionario database_get(char[20] nome_tabela, char[20] nome_campo, char[150] valor_campo){
//     return funcionario;
// }

// Funcionario database_create(char[20] nome_tabela, Funcionario* funcionario){
//     return funcionario;
// }

// int database_update(char[20] nome_tabela, Funcionario* funcionario){
//     return 1;
// }

// int database_delete(char[20] nome_tabela, Funcionario* funcionario){
//     return 1;
// }