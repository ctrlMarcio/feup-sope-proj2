#include "client_fifo.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../util/error.h"
#include "../util/utils.h"

int open_public_fifo(char *fifo_name)
{
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Server unavailable, aborting\n");
        exit(CONNECTION_ERROR);
    }

    return fd;
}

void create_private_fifo(pid_t pid, pthread_t tid, char *fifo_name)
{
    sprintf(fifo_name, "%s%d.%ld", FIFO_FOLDER, pid, tid);

    if (mkfifo(fifo_name, FIFO_PERMS) < 0)
    {
        perror("Create private FIFO");
        exit(FIFO_CREATION_ERROR);
    }
}

int open_private_fifo(char *fifo_name)
{
    int fd;

    if ((fd = open(fifo_name, O_RDONLY)) == -1)
    {
        perror("Open private FIFO");
        unlink(fifo_name);
        exit(FIFO_CREATION_ERROR);
    }

    return fd;
}

void destroy_private_fifo(int fd, char *name)
{
    close(fd);
    unlink(name);
}