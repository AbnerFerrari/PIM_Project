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

                printf("Nome: ");
                scanf("%s", func.nome);

                printf("CPF: ");
                scanf("%s", func.cpf);

                printf("Senha: ");
                scanf("%s", func.senha);

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