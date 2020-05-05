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
#define ARGUMENT_COUNT 8

/**
 * @brief Reads the console arguments.
 * 
 * @param argc                  the number of arguments
 * @param argv                  the argument array
 * @param number_of_seconds     a pointer to the number of seconds
 * @param number_of_places      a pointer to the number of places
 * @param number_of_threads     a pointer to the number of threads
 * @param fifoname              a pointer to the FIFO name
 */
void read_arguments(int argc, char *argv[], int *number_of_seconds, int *number_of_places, int *number_of_threads, char *fifoname);

#endif
