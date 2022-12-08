#define clear_screen() printf("\033[H\033[J")

int create_sock_connection();

void send_message(int sock, char* message);

int read_answer(int sock, char* buffer, int buffer_size);

void close_sock_connection(int sock);

int delete(char* table_name, int id);
void save(char* nome_tabela, char* entity);
void list(char* nome_tabela);
void get(char* table_name, int id);
void get_report(char* report_name, char* buffer);
void edit(char* nome_tabela, char* entity);