/**
 * @file    client.c
 * @brief   Runs the main client operations.
 * @date    2020-04-28
 */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../util/error.h"
#include "../util/utils.h"
#include "client_arguments.h"
#include "client_fifo.h"
#include "client_thread.h"

/**
 * @brief   The delay, in milliseconds, between each request thread.
 */
const int MILLIS_DELAY = 10;

/**
 * @brief   Sets up the SIGALARM.
 * 
 * @param nsecs     The number of seconds to set the alarm to
 */
void setup_signals(int nsecs);

/**
 * @brief   Terminates the program.
 * @details Is called when SIGALARM is received. Can be called by any signal.
 * 
 * @param signo     The signal identification number
 */
void terminate(int signo);

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
    setup_signals(nsecs);

    // setup random seed
    srand(time(NULL));

    pthread_t thread;
    unsigned long request_count = 0;
    while (1)
    {
        if (pthread_create(&thread, NULL, request_handler, &request_count))
        {
            perror("Creating thread");
            exit(INIT_THREAD_ERROR);
        }
        request_count++;
        pthread_detach(thread);
        usleep(MILLIS_DELAY * 1000); // usleep is in microseconds
    }

    // only for precaution, the regular termination of the program is in the function "terminate"
    pthread_exit(0);
}

void setup_signals(int nsecs)
{
    struct sigaction action;
    action.sa_handler = terminate;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);

    alarm(nsecs);

    sigset_t set;
    sigemptyset(&set);
    pthread_sigmask(SIGPIPE, &set, NULL);    
}

void terminate(int signo)
{
    close(request_fd);
    pthread_exit(0);
}