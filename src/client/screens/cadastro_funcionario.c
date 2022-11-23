#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/client.h"
#include "../../types/funcionario.h"

void list(char* nome_tabela);
void save(char* nome_tabela, Funcionario* funcionario);
// Funcionario database_get(char[20] nome_tabela, char[20] nome_campo, char[150] valor_campo);
// int database_update(char[20] nome_tabela, Funcionario* funcionario);
// int database_delete(char[20] nome_tabela, Funcionario* funcionario);

void cadastro_funcionario(int sock){
    int option;
    
    do
    {
        clear_screen();
        printf("1 - Listar\n2 - Cadastrar novo funcionário\n3 - Editar funcionário\n4 - Excluir funcionário\n5 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        clear_screen();
        switch (option)
        {
            case 1: // listar funcionários
                list("funcionarios"); // nome da tabela
                printf("\nPressione ENTER para retornar");
                
                scanf("%*c");
                
                break;
            case 2:
                Funcionario func = {};

                printf("Nome (%d caracteres): ", FUNCIONARIO_NAME_SIZE - 1);
                char name[FUNCIONARIO_NAME_SIZE];
                bzero(name, FUNCIONARIO_NAME_SIZE);
                scanf("%[^\n]", name);
                sprintf(func.nome, FUNCIONARIO_NAME_FORMAT_IN, name);
                
                printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
                char cpf[FUNCIONARIO_CPF_SIZE];
                bzero(cpf, FUNCIONARIO_CPF_SIZE);
                scanf("%s", cpf);
                sprintf(func.cpf, FUNCIONARIO_CPF_FORMAT_IN, cpf);
                
                printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
                char password[FUNCIONARIO_PASSWORD_SIZE];
                bzero(password, FUNCIONARIO_PASSWORD_SIZE);
                scanf("%s", password);
                sprintf(func.senha, FUNCIONARIO_PASSWORD_FORMAT_IN, password);

                save("funcionarios", &func);

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
                printf("Informe o CPF do funcionário a ser exluido: ");
                
                Funcionario funcionario = {};
                scanf("%s", funcionario.cpf);

                delete("funcionarios", funcionario.cpf);
                break;
            case 5:
                option = 0;
                break;
            
            default:
                break;
        }
    } while (option != 0);
}