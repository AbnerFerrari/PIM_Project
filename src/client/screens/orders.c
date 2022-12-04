#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/server_access.h"
#include "../../types/order.h"

void orders(){
    int option;
    
    do
    {
        system("clear");
        printf("1 - Listar\n2 - Cadastrar novo pedido\n3 - Editar pedido\n4 - Excluir pedido\n5 - Pesquisar pedido específico\n6 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        system("clear");
        
        int id = 0;

        switch (option)
        {
            case 1:
                list("pedidos");
                printf("\nPressione ENTER para retornar");
                
                scanf("%*c");
                
                break;
            case 2:
                Order order = {};

                printf("Id do vendedor: ");
                scanf("%d%*c", &order.userId);
                
                printf("Nome do cliente (%d caracteres): ", ORDER_CLIENT_NAME_SIZE - 1);
                scanf("%s[^\n]", order.client);
                
                printf("Nome do produto (%d caracteres): ", ORDER_PRODUCT_NAME_SIZE - 1);
                scanf("%s[^\n]", order.product);

                printf("Quantidade: ");
                scanf("%d%*c", &order.quantity);

                printf("Valor unitário: ");
                scanf("%f%*c", &order.unit_price);

                char entity[sizeof(Order)];
                sprintf(entity, "%d %s %s %d %f", order.userId, order.client, order.product, order.quantity, order.unit_price);
                save("pedidos", entity);

                break;
            case 3:
                // printf("Id do funcionário a ser editado: ");
                // scanf("%d%*c", &func.id);

                // printf("Nome (%d caracteres): ", FUNCIONARIO_NAME_SIZE - 1);
                // scanf("%s", func.nome);
                
                // printf("CPF (%d caracteres): ", FUNCIONARIO_CPF_SIZE - 1);
                // scanf("%s", func.cpf);
                
                // printf("Senha (%d caracteres): ", FUNCIONARIO_PASSWORD_SIZE - 1);
                // scanf("%s", func.senha);

                // char funcionario[sizeof(User)];
                // sprintf(funcionario, "%d %s %s %s", func.id, func.nome, func.cpf, func.senha);
                // edit("funcionarios", funcionario);

                break;
            case 4:
                // while (id <= 0)
                // {
                //     printf("Informe o Id do funcionário a ser exluido: ");
                //     scanf("%d%*c", &id);
                // }

                // delete("funcionarios", id);
                break;
            case 5:
                // while (id <= 0)
                // {
                //     printf("Informe o Id do funcionário a ser visualizado: ");
                //     scanf("%d%*c", &id);
                // }

                // system("clear");
                // get("funcionarios", id);
                // printf("Pressione ENTER para retornar!");
                // scanf("%*c");
                break;
            case 6:
                option = 0;
                break;
            default:
                break;
        }
    } while (option != 0);
}