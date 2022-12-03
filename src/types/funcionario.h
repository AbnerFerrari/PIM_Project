#define FUNCIONARIO_NAME_SIZE 51
#define FUNCIONARIO_CPF_SIZE 12
#define FUNCIONARIO_PASSWORD_SIZE 21

#define FUNCIONARIO_PRETTY_FORMAT_WITH_ID "Id: %d\nNome: %s\nCPF: %s\nSenha: %s\n\n\n"
#define FUNCIONARIO_PRETTY_FORMAT "Nome: %s\nCPF: %s\nSenha: %s\n\n\n"

typedef struct _Funcionario{
    int id;
    char nome[FUNCIONARIO_NAME_SIZE];
    char cpf[FUNCIONARIO_CPF_SIZE];
    char senha[FUNCIONARIO_PASSWORD_SIZE];
} Funcionario;