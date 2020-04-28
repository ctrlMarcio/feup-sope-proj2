/**
 * @file    server.h
 * @brief   Methods related to the server side 
 * @date    2020-04-28
 */
#ifndef SERVER_H
#define SERVER_H

#define _GNU_SOURCE

#include "../util/utils.h"

/**
 * @brief   The number of arguments
 */
#define ARGUMENT_COUNT 4

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname);

int init_public_fifo(char *fifoname);

void close_public_fifo();

void terminate(int signo);

void init_alarm();

#endif