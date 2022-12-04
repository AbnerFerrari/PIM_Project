#define FUNCIONARIO_NAME_SIZE 51
#define FUNCIONARIO_CPF_SIZE 12
#define FUNCIONARIO_PASSWORD_SIZE 21

#define FUNCIONARIO_PRETTY_FORMAT "Id: %d\nNome: %s\nCPF: %s\nSenha: %s\n\n\n"

typedef struct _User{
    int id;
    char nome[FUNCIONARIO_NAME_SIZE];
    char cpf[FUNCIONARIO_CPF_SIZE];
    char senha[FUNCIONARIO_PASSWORD_SIZE];
} User;