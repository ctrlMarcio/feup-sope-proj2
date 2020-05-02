#include "server_fifo.h"

int init_public_fifo(char *fifoname)
{
    if (mkfifo(fifoname, FIFO_PERMS) < 0)
    {
        perror("Can't create FIFO");
        exit(FIFO_CREATION_ERROR);
    }
    else
    {
        fprintf(stderr, "FIFO '%s' successfully created\n", fifoname);
    }

    int fd;

    if ((fd = open(fifoname, O_RDONLY)) != -1)
        fprintf(stderr, "FIFO '%s' [%d] openned in READONLY mode\n", fifoname, fd);
    else
    {
        perror("Could not open FIFO");
        exit(FIFO_CREATION_ERROR);
    }

    return fd;
}

void close_public_fifo(int fd)
{
    close(fd);
}

int open_private_fifo(char *fifoname, query *query)
{
    int fd = open(fifoname, O_WRONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Server unavailable, aborting\n");
        register_operation(GAVUP, query);
        exit(CONNECTION_ERROR);
    }

    return fd;
}