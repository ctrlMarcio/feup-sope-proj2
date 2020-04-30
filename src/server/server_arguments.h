#ifndef SERVER_ARGUMENTS_H
#define SERVER_ARGUMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../util/error.h"
#include "../util/utils.h"

/**
 * @brief   The number of arguments
 */
#define ARGUMENT_COUNT 4

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname);

#endif