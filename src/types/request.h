#define REQUEST_FORMAT "%-6s %-20s %-200s"
#define REQUEST_ACTION_FORMAT_IN "%-6s\0"
#define REQUEST_TABLE_FORMAT_IN "%-20s\0"
#define REQUEST_BODY_FORMAT_IN "%-200s\0]"

typedef struct request {
    char action[7];
    char table[21];
    char body[201];
    int running_thread_index;
    int sock;
} Request;
