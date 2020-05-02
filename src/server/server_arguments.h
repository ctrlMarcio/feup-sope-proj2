/**
 * @file    server_arguments.h
 * @brief   Methods related to argument handling.
 * @date    2020-05-02
 */
#ifndef SERVER_ARGUMENTS_H
#define SERVER_ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../util/error.h"
#include "../util/utils.h"

/**
 * @brief   The number of arguments.
 */
#define ARGUMENT_COUNT 4

/**
 * @brief Reads the console arguments.
 * 
 * @param argc                  the number of arguments
 * @param argv                  the argument array
 * @param number_of_seconds     a pointer to the number of seconds
 * @param fifoname              a pointer to the FIFO name
 */
void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname);

#endif