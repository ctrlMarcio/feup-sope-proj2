/**
 * @file    server.h
 * @brief    
 * @date    2020-04-28
 */
#ifndef SERVER_H
#define SERVER_H

#include <string.h>
#include "../util/utils.h"

#define ARGUMENT_COUNT 4

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname);

#endif