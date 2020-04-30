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
        printf("FIFO '%s' successfully created\n", fifoname);
    }

    int fd;

    if ((fd = open(fifoname, O_RDONLY)) != -1)
        printf("FIFO '%s' [%d] openned in READONLY mode\n", fifoname, fd);
    else
    {
        perror("Could not open FIFO");
        exit(FIFO_CREATION_ERROR);
    }

    return fd;
}

void close_public_fifo(int fifo_fd, char *fifoname)
{
    close(fifo_fd);
    unlink(fifoname);
}

int open_private_fifo(char *fifo_name)
{
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Server unavailable, aborting\n");
        exit(CONNECTION_ERROR);
    }

    return fd;
}