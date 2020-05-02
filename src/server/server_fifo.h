/**
 * @file    server_fifo.h
 * @brief   Methods related to FIFO handling.
 * @date    2020-05-02
 */
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

/**
 * @brief   Initializes the public FIFO.
 * 
 * @param fifoname  the FIFO name
 * @return int      the FIFO file descriptor
 */
int init_public_fifo(char *fifoname);

/**
 * @brief   Opens a private FIFO.
 * 
 * @param fifoname  the FIFO name
 * @param query     the request
 * @return int      the FIFO file descriptor
 */
int open_private_fifo(char *fifoname, query *query);

#endif