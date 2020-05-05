#include "server_arguments.h"

void read_arguments(int argc, char *argv[], int *number_of_seconds, int *number_of_places, int *number_of_threads, char *fifoname)
{
    // if (argc != ARGUMENT_COUNT)
    // {
    //     fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
    //     exit(ARGS_ERROR);
    // }

    // if (strcmp(argv[1], "-t"))
    // {
    //     fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
    //     exit(ARGS_ERROR);
    // }

    // *number_of_seconds = atoi(argv[2]);

    // if (*number_of_seconds <= 0)
    // {
    //     fprintf(stderr, "The number of seconds must be a number greater than 0\n");
    //     exit(ARGS_ERROR);
    // }

    // if (!strcmp(argv[ARGUMENT_COUNT - 1], ".") || !strcmp(argv[ARGUMENT_COUNT - 1], ".."))
    // {
    //     fprintf(stderr, "The FIFO name is invalid\n");
    //     exit(ARGS_ERROR);
    // }

    // strcpy(fifoname, FIFO_FOLDER);
    // strcat(fifoname, argv[ARGUMENT_COUNT - 1]);

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
        {
            if (argc > current_arg + 1)
            {
                *number_of_seconds = atoi(argv[current_arg + 1]);

                if (*number_of_seconds <= 0)
                {
                    fprintf(stderr, "The number of seconds must be a number greater than 0\n");
                    exit(ARGS_ERROR);
                }

                flags[0] = true;
                current_arg += 2;
            }
            else
            {
                fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
                exit(ARGS_ERROR);
            }
        }
        // Reads number of places
        else if (!strcmp("-l", argv[current_arg]))
        {
            if (argc > current_arg + 1)
            {
                *number_of_places = atoi(argv[current_arg + 1]);

                if (*number_of_places <= 0)
                {
                    fprintf(stderr, "The number of places must be a number greater than 0\n");
                    exit(ARGS_ERROR);
                }

                flags[1] = true;
                current_arg += 2;
            }
            else
            {
                fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
                exit(ARGS_ERROR);
            }
        }
        // Reads number of threads
        else if (!strcmp("-n", argv[current_arg]))
        {
            if (argc > current_arg + 1)
            {
                *number_of_threads = atoi(argv[current_arg + 1]);

                if (*number_of_threads <= 0)
                {
                    fprintf(stderr, "The number of threads must be a number greater than 0\n");
                    exit(ARGS_ERROR);
                }

                flags[2] = true;
                current_arg += 2;
            }
            else
            {
                fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
                exit(ARGS_ERROR);
            }
        }
        else
        {
            if (!strcmp(argv[current_arg], ".") || !strcmp(argv[current_arg], ".."))
            {
                fprintf(stderr, "The FIFO name is invalid\n");
                exit(ARGS_ERROR);
            }

            strcpy(fifoname, FIFO_FOLDER);
            strcat(fifoname, argv[current_arg]);
            flags[3] = true;
            current_arg++;
        }
    }

    if (!(flags[0] && flags[1] && flags[2] && flags[3]))
    {
        fprintf(stderr, "Usage: %s [-t <number of seconds>] [-l <number of places>] [-n <number of threads>] [<fifoname>]\n", argv[0]);
        exit(ARGS_ERROR);
    }
}
