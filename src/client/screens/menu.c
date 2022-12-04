#include <stdio.h>
#include <stdlib.h>
#include "../headers/server_access.h"
#include "../headers/users.h"
#include "../headers/orders.h"

void menu(){
    int option;
    
    do
    {
        clear_screen();
        
        printf("1 - Funcionários\n2 - Pedidos\n3 - Sair\nEscolha uma opção: ");
        scanf("%d%*c", &option);

        switch (option)
        {
            case 1:
                users();
                break;
            case 2:
                orders(); 
                break;
            case 3: // Tela de cadastro de perfil
                option = 0;
                break;
            case 4: // Tela de cadastro de vendas
                break;
            case 5: // Tela relatório
                break;
            default:
                break;
        }
    } while (option != 0);
}

/**
 * 1 - Tela de login - OK
	2 - Tela de menu inicial - Abner
	3 - Tela de cadastro de clientes - Telefone, cpf e nome - Daniel, Jean
	4 - Tela de cadastro de funcionários/usario - cpf, senha, nome - Daniel, Jean
	5 - Tela de cadastro de perfil - Deixa por ultimo
	6 - Telas de relatórios (em arquivo txt): - Daniel, Jean
		6.1 - Vendas por funcionário por período
		6.2 - Vendas por período
	7 - Tela de cadastro de vendas - Produto, valor, cliente - Abner
*/