#define ORDER_BY_USER_PRETTY_FORMAT "Vendedor: %s\nPedidos realizados: %d\nValor total vendido: %.2f\n\n\n"

typedef struct _OrderByUser{
    char user[50];
    int quantity_orders;
    float total_orders_price;
} OrderByUser;