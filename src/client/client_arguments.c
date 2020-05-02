#include "client_arguments.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../util/error.h"
#include "../util/utils.h"

int read_args(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    // verifies if the nsecs arguments is ok
    int nsecs = atoi(argv[2]);
    if (nsecs <= 0)
    {
        fprintf(stderr, "The number of seconds must be a number greater than 0\n");
        exit(ARGS_ERROR);
    }

    return nsecs;
}

void read_fifo_name(char *dst, char *fifo_name)
{
    if (!strcmp(fifo_name, "..") || !strcmp(fifo_name, "."))
    {
        fprintf(stderr, "The FIFO name is invalid\n");
        exit(ARGS_ERROR);
    }

    strcpy(dst, FIFO_FOLDER);
    strcat(dst, fifo_name);
}