#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include "error.h"

/* TODO DELETE */ #define MAX_FIFO_NAME 1024

/**
 * \brief   The name of the folder where FIFOs are to be interchanged.
 */
/* TODO DELETE */ const char *FIFO_FOLDER = "/tmp/";

int read_args(int argc, char *argv[]);
void read_fifo_name(char *dst, char *fifoName);

int main(int argc, char *argv[])
{
    // read arguments
    int nsecs = read_args(argc, argv);

    // save public fifo name
    char publicFifo[MAX_FIFO_NAME]; // TODO save this globally
    read_fifo_name(publicFifo, argv[3]);

    // TEST
    printf("%s\n", publicFifo);

    // send sigalarm

    // open public fifo

    // send threads

    // free resources and delete private fifo

    pthread_exit(0);
}

/**
 * \brief   Reads the arguments passed by the terminal.
 * 
 * \param argc  The argument count
 * \param argv  The arguments array
 * \return int  The number of seconds for the program to run, "nsecs"
 */
int read_args(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <-t nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s <-t nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    // verifies if the nsecs arguments is ok
    int nsecs = atoi(argv[2]);
    if (nsecs <= 0) {
        fprintf(stderr, "Invalid nsecs\n");
        exit(ARGS_ERROR);
    }

    return nsecs;
}

/**
 * \brief   Appends and updates the FIFO's name.
 * 
 * \param dst       The string to read the name to
 * \param fifoName  The name of the FIFO.
 */
void read_fifo_name(char *dst, char *fifoName)
{
    if (!strcmp(fifoName, "..") || !strcmp(fifoName, "."))
    {
        fprintf(stderr, "Invalid FIFO name\n");
        exit(ARGS_ERROR);
    }

    strcpy(dst, FIFO_FOLDER);
    strcat(dst, fifoName);
}
