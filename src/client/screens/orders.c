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
        Order order = {};
        char entity[sizeof(Order)];
        bzero(entity, sizeof(Order));

        switch (option)
        {
            case 1:
                list("pedidos");
                printf("\nPressione ENTER para retornar");
                
                scanf("%*c");
                
                break;
            case 2:

                printf("Id do vendedor: ");
                scanf("%d%*c", &order.userId);
                
                printf("Nome do cliente (%d caracteres): ", ORDER_CLIENT_NAME_SIZE - 1);
                scanf("%[^\n]%*c", order.client);
                
                printf("Nome do produto (%d caracteres): ", ORDER_PRODUCT_NAME_SIZE - 1);
                scanf("%[^\n]%*c", order.product);

                printf("Quantidade: ");
                scanf("%d%*c", &order.quantity);

                printf("Valor unitário: ");
                scanf("%f%*c", &order.unit_price);

                sprintf(entity, "%d %s; %s; %d %f", order.userId, order.client, order.product, order.quantity, order.unit_price);
                save("pedidos", entity);

                break;
            case 3:
                printf("Id do pedido a ser editado: ");
                scanf("%d%*c", &order.id);

                printf("Id do vendedor: ");
                scanf("%d%*c", &order.userId);
                
                printf("Nome do cliente (%d caracteres): ", ORDER_CLIENT_NAME_SIZE - 1);
                scanf("%[^\n]%*c", order.client);
                
                printf("Nome do produto (%d caracteres): ", ORDER_PRODUCT_NAME_SIZE - 1);
                scanf("%[^\n]%*c", order.product);

                printf("Quantidade: ");
                scanf("%d%*c", &order.quantity);

                printf("Valor unitário: ");
                scanf("%f%*c", &order.unit_price);

                sprintf(entity, "%d %d %s; %s; %d %f", order.id, order.userId, order.client, order.product, order.quantity, order.unit_price);

                edit("pedidos", entity);

                break;
            case 4:
                while (id <= 0)
                {
                    printf("Informe o Id do pedido a ser exluido: ");
                    scanf("%d%*c", &id);
                }

                delete("pedidos", id);
                break;
            case 5:
                while (id <= 0)
                {
                    printf("Informe o Id do pedido a ser visualizado: ");
                    scanf("%d%*c", &id);
                }

                system("clear");
                get("pedidos", id);
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