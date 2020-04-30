#include "message.h"

void register_operation(char *op_name, query *query)
{
    char message[MAX_MESSAGE_SIZE];

    time_t instant = time(NULL);

    sprintf(message, "%lu; %lu; %d; %ld; %d; %ld; %s\n", instant, query->i, query->pid, query->tid, query->dur, query->pl, op_name);

    write(STDOUT_FILENO, message, strlen(message));
}