typedef struct _Funcionario{
    char nome[51];
    char cpf[12];
    char senha[21];
} Funcionario;

#define FUNCIONARIO_FORMAT_IN "%-51s%-12s%-21s"
#define FUNCIONARIO_FORMAT_OUT "Nome: %s\nCPF: %s\nSenha: %s\n\n\n"