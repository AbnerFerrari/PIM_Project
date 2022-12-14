#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/server_access.h"
#include "../../types/user.h"

void users(){
    int option;
    
    do
    {
        system("clear");
        printf("1 - Listar\n2 - Cadastrar novo funcionário\n3 - Editar funcionário\n4 - Excluir funcionário\n5 - Pesquisar funcionario específico\n6 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        system("clear");
        
        int id = 0;

        switch (option)
        {
            case 1:
                list("funcionarios");
                printf("\nPressione ENTER para retornar");
                
                scanf("%*c");
                
                break;
            case 2:
                User func = {};

                printf("Nome (%d caracteres): ", FUNCIONARIO_NAME_SIZE - 1);
                scanf("%[^\n]%*c", func.nome);
                
                printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
                scanf("%s", func.cpf);
                
                printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
                scanf("%s", func.senha);

                char* entity = malloc(strlen(func.nome) + strlen(func.cpf) + strlen(func.senha));
                sprintf(entity, "%s; %s %s", func.nome, func.cpf, func.senha);
                save("funcionarios", entity);
                free(entity);

                break;
            case 3:
                printf("Id do funcionário a ser editado: ");
                scanf("%d%*c", &func.id);

                printf("Nome (%d caracteres): ", FUNCIONARIO_NAME_SIZE - 1);
                scanf("%[^\n]%*c", func.nome);
                
                printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
                scanf("%s", func.cpf);
                
                printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
                scanf("%s", func.senha);

                char funcionario[sizeof(User)];
                sprintf(funcionario, "%d %s; %s %s", func.id, func.nome, func.cpf, func.senha);
                edit("funcionarios", funcionario);

                break;
            case 4:
                while (id <= 0)
                {
                    printf("Informe o Id do funcionário a ser exluido: ");
                    scanf("%d%*c", &id);
                }

                delete("funcionarios", id);
                break;
            case 5:
                while (id <= 0)
                {
                    printf("Informe o Id do funcionário a ser visualizado: ");
                    scanf("%d%*c", &id);
                }

                system("clear");
                get("funcionarios", id);
                printf("Pressione ENTER para retornar!");
                scanf("%*c");
                break;
            case 6:
                option = 0;
                break;
            default:
                break;
        }
    } while (option != 0);
}