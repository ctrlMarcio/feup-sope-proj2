#ifndef SERVER_FIFO_H
#define SERVER_FIFO_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "../util/error.h"
#include "../util/utils.h"

int init_public_fifo(char *fifoname);

void close_public_fifo(int fifo_fd, char *fifoname);

int open_private_fifo(char *fifo_name);

#endif