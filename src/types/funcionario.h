#define FUNCIONARIO_NAME_SIZE 51
#define FUNCIONARIO_CPF_SIZE 12
#define FUNCIONARIO_PASSWORD_SIZE 21

#define FUNCIONARIO_FORMAT_IN "%d %s %s %s"
#define FUNCIONARIO_NAME_FORMAT_IN "%-51s"
#define FUNCIONARIO_CPF_FORMAT_IN "%-12s"
#define FUNCIONARIO_PASSWORD_FORMAT_IN "%-21s"

#define FUNCIONARIO_FORMAT_OUT "%50[^\n] %11s %20s"
#define FUNCIONARIO_PRETTY_FORMAT "Nome: %s\nCPF: %s\nSenha: %s\n\n\n"

typedef struct _Funcionario{
    int id;
    char nome[FUNCIONARIO_NAME_SIZE];
    char cpf[FUNCIONARIO_CPF_SIZE];
    char senha[FUNCIONARIO_PASSWORD_SIZE];
} Funcionario;