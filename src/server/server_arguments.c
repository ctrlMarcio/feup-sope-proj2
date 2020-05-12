#include "server_arguments.h"

void read_single_argument(int argc, char *argv[], int *current_arg, int *value, bool *flags, char *message, int flag_index);

void read_fifoname(int argc, char *argv[], int *current_arg, char *value, bool *flags, char *message, int flag_index);

void read_arguments(int argc, char *argv[], int *number_of_seconds, int *number_of_places, int *number_of_threads, char *fifoname)
{
    if (argc != ARGUMENT_COUNT)
    {
        fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
        exit(ARGS_ERROR);
    }

    /**
     * 0 - number of seconds
     * 1 - number of places
     * 2 - number of threads
     * 3 - fifoname
     */
    bool flags[4] = {false};

    int current_arg = 1;

    // TODO refactor ifs in methods

    while (current_arg < argc)
    {
        // Reads number of seconds
        if (!strcmp("-t", argv[current_arg]))
            read_single_argument(argc, argv, &current_arg, number_of_seconds, flags, "The number of seconds must be a number greater than 0", 0);
        // Reads number of places
        else if (!strcmp("-l", argv[current_arg]))
            read_single_argument(argc, argv, &current_arg, number_of_places, flags, "The number of places must be a number greater than 0", 1);
        // Reads number of threads
        else if (!strcmp("-n", argv[current_arg]))
            read_single_argument(argc, argv, &current_arg, number_of_threads, flags, "The number of threads must be a number greater than 0", 2);
        // Reads fifoname
        else
            read_fifoname(argc, argv, &current_arg, fifoname, flags, "", 3);
    }

    if (!(flags[0] && flags[1] && flags[2] && flags[3]))
    {
        fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
        exit(ARGS_ERROR);
    }
}

void read_single_argument(int argc, char *argv[], int *current_arg, int *value, bool *flags, char *message, int flag_index)
{
    if (argc > *current_arg + 1)
    {
        *value = atoi(argv[*current_arg + 1]);

        if (*value <= 0)
        {
            fprintf(stderr, "%s\n", message);
            exit(ARGS_ERROR);
        }

        flags[flag_index] = true;
        *current_arg += 2;
    }
    else
    {
        fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
        exit(ARGS_ERROR);
    }
}

void read_fifoname(int argc, char *argv[], int *current_arg, char *value, bool *flags, char *message, int flag_index)
{
    if (!strcmp(argv[*current_arg], ".") || !strcmp(argv[*current_arg], ".."))
    {
        fprintf(stderr, "%s\n", message);
        exit(ARGS_ERROR);
    }

    strcpy(value, FIFO_FOLDER);
    strcat(value, argv[*current_arg]);
    flags[flag_index] = true;
    (*current_arg)++;
}
