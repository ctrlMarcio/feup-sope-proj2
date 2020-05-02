#include "server_arguments.h"

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname)
{
    if (argc != ARGUMENT_COUNT)
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    *number_of_seconds = atoi(argv[2]);

    if (*number_of_seconds <= 0)
    {
        fprintf(stderr, "The number of seconds must be a number greater than 0\n");
        exit(ARGS_ERROR);
    }

    if (!strcmp(argv[ARGUMENT_COUNT - 1], ".") || !strcmp(argv[ARGUMENT_COUNT - 1], ".."))
    {
        fprintf(stderr, "The FIFO name is invalid\n");
        exit(ARGS_ERROR);
    }

    strcpy(fifoname, FIFO_FOLDER);
    strcat(fifoname, argv[ARGUMENT_COUNT - 1]);
}