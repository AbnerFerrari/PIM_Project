typedef struct _Funcionario{
    char nome[51];
    char cpf[12];
    char senha[21];
} Funcionario;

#define FUNCIONARIO_SERIALIZE_FORMAT "%-51s%-12s%-21s"