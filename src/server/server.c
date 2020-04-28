#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[])
{
    // Argument reading and checking
    int number_of_seconds;
    char fifoname[MAX_FIFO_NAME_SIZE];

    read_arguments(argc, argv, &number_of_seconds, fifoname);

    // Alarm

    // Thread

    exit(0);
}

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname)
{
    if (argc != ARGUMENT_COUNT)
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        // TODO replace the exit code
        exit(1);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        // TODO replace the exit code
        exit(1);
    }

    *number_of_seconds = atoi(argv[2]);

    if (*number_of_seconds <= 0)
    {
        fprintf(stderr, "The number of seconds must be greater than 0\n");
        // TODO replace the exit code
        exit(1);
    }

    strcpy(fifoname, argv[ARGUMENT_COUNT - 1]);

    if (!strcmp(fifoname, ".") || !strcmp(fifoname, ".."))
    {
        fprintf(stderr, "The fifo name is invalid\n");
        // TODO replace the exit code
        exit(1);
    }
}