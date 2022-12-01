#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/client.h"
#include "../../types/funcionario.h"

void cadastro_funcionario(int sock){
    int option;
    
    do
    {
        system("clear");
        printf("1 - Listar\n2 - Cadastrar novo funcionário\n3 - Editar funcionário\n4 - Excluir funcionário\n5 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        system("clear");
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
                scanf("%s", func.nome);
                
                printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
                scanf("%s", func.cpf);
                
                printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
                scanf("%s", func.senha);

                char* entity = malloc(strlen(func.nome) + strlen(func.cpf) + strlen(func.senha));
                sprintf(entity, "%s %s %s", func.nome, func.cpf, func.senha);
                save("funcionarios", entity);
                free(entity);

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
                int id = 0;
                while (id <= 0)
                {
                    printf("Informe o Id do funcionário a ser exluido: "); /* code */
                    scanf("%d", &id);
                }

                delete("funcionarios", id);
                break;
            case 5:
                option = 0;
                break;
            
            default:
                break;
        }
    } while (option != 0);
}