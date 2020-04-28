/**
 * @file    client.c
 * @brief   Runs the main client operations.
 * @date    2020-04-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../util/error.h"
#include "../util/utils.h"

/**
 * @brief   Saves the file descriptor of the public fifo.
 */
int request_fd;

/**
 * @brief   Reads the arguments passed by the terminal.
 * 
 * @param argc  The argument count
 * @param argv  The arguments array
 * @return int  The number of seconds for the program to run, "nsecs"
 */
int read_args(int argc, char *argv[]);

/**
 * @brief   Appends and updates the FIFO's name.
 * 
 * @param dst       The string to read the name to
 * @param fifo_name The name of the FIFO
 */
void read_fifo_name(char *dst, char *fifo_name);

/**
 * @brief   Sets up the SIGALARM.
 * 
 * @param nsecs     The number of seconds to set the alarm to
 */
void setup_alarm(int nsecs);

/**
 * @brief   Terminates the program.
 * @details Is called when SIGALARM is received. Can be called by any signal.
 * 
 * @param signo     The signal identification number
 */
void terminate(int signo);

/**
 * @brief   Opens the 
 * 
 * @param fifo_name 
 * @return int 
 */
int open_public_fifo(char *fifo_name);

int main(int argc, char *argv[])
{
    // read arguments
    int nsecs = read_args(argc, argv);

    // save public fifo name
    char public_fifo[MAX_FIFO_NAME_SIZE];
    read_fifo_name(public_fifo, argv[3]);

    // open public fifo
    request_fd = open_public_fifo(public_fifo);

    // send sigalarm
    setup_alarm(nsecs);

    while (1) {
        // TODO send threads
        printf("running ...\n");    // TEST
        sleep(1);                   // TEST
    }

    // only for precaution, the regular termination of the program is in the function "terminate"
    pthread_exit(0);
}

int read_args(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s -t <nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    // verifies if the nsecs arguments is ok
    int nsecs = atoi(argv[2]);
    if (nsecs <= 0)
    {
        fprintf(stderr, "Invalid nsecs\n");
        exit(ARGS_ERROR);
    }

    return nsecs;
}

void read_fifo_name(char *dst, char *fifo_name)
{
    if (!strcmp(fifo_name, "..") || !strcmp(fifo_name, "."))
    {
        fprintf(stderr, "Invalid FIFO name\n");
        exit(ARGS_ERROR);
    }

    strcpy(dst, FIFO_FOLDER);
    strcat(dst, fifo_name);
}

void setup_alarm(int nsecs)
{
    struct sigaction action;
    action.sa_handler = terminate;
    action.sa_flags = 0;
    sigfillset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);

    alarm(nsecs);
}

void terminate(int signo)
{
    close(request_fd);
    // TODO free resources and delete private fifo
    pthread_exit(0);
}

int open_public_fifo(char *fifo_name) {
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1) {
        fprintf(stderr, "Server unavailable, aborting\n");
        exit(CONNECTION_ERROR);
    }

    return fd;
}
