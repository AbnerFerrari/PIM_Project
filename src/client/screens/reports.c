#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/server_access.h"
#include "../../types/order_by_user.h"

void reports(){
    int option;
    
    do
    {
        system("clear");
        printf("1 - Pedidos por vendedor\n2 - Voltar à tela anterior\nEscolha uma opção: ");
        scanf("%d%*c", &option);
        system("clear");
        
        int id = 0;

        switch (option)
        {
            case 1:
                char buffer[sizeof(OrderByUser) * 30];
                get_report("report_orders_by_user",buffer);
                printf("Arquivo chamado \"pedidos_por_vendedor.txt\" gerado com sucesso!.\n\nPressione ENTER para retornar");
                
                scanf("%*c");
                
                break;
            case 2:
                option = 0;
                break;
            default:
                break;
        }
    } while (option != 0);
}