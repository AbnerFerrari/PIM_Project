#define ORDER_CLIENT_NAME_SIZE 51
#define ORDER_PRODUCT_NAME_SIZE 31

#define ORDER_PRETTY_FORMAT "Id: %d\nVendedor: %s\nCliente: %s\nProduto: %s\nQuantidade: %s\nValor unitário: R$ %.2f\nTotal: R$ %.2f\n\n\n"

typedef struct _Order{
    int id;
    int userId;
    char client[ORDER_CLIENT_NAME_SIZE];
    char product[ORDER_PRODUCT_NAME_SIZE];
    int quantity;
    float unit_price;
} Order;