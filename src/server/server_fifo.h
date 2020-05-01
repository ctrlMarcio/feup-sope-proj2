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
#include "../util/message.h"

int init_public_fifo(char *fifoname);

void unlink_public_fifo(char *fifoname);

void close_public_fifo(int fd);

int open_private_fifo(char *fifoname, query *query);

#endif