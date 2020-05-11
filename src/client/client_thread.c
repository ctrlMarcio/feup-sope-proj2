#include "client_thread.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#include "client_fifo.h"
#include "../util/utils.h"
#include "../util/message.h"

/**
 * @brief   The name of the private FIFO.
 */
char private_fifo_name[MAX_FIFO_NAME_SIZE];

/**
 * @brief   Sets up the signal handlers for the individual thread.
 */
void setup_thread_signals();

/**
 * @brief   The handler for when a SIGPIPE is received. 
 * 
 * @param   signo the signal code
 */
void sigpipe_handler(int signo);

void *request_handler(void *arg)
{
    // setup signals
    setup_thread_signals();

    // get values
    unsigned long i = *(int *)arg;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    int dur = rand() % MAX_DURATION;
    long pl = -1;

    dur = 50;

    // create FIFO
    char fifo_name[MAX_FIFO_NAME_SIZE];
    create_private_fifo(pid, tid, fifo_name);

    strcpy(private_fifo_name, fifo_name);

    // send request
    query query = {i, pid, tid, dur, pl};
    if (write(request_fd, &query, sizeof(query)) == -1) // there's a problem with the public FIFO
    {
        register_operation(FAILD, &query);
        unlink(fifo_name);
        return NULL;
    }
    register_operation(IWANT, &query);

    // open FIFO
    int fd = open_private_fifo(fifo_name);

    // receive response
    if (read(fd, &query, sizeof(query)) <= 0)
    {
        register_operation(FAILD, &query);
        // destroy FIFO
        destroy_private_fifo(fd, fifo_name);
        return NULL;
    }

    // verify if failed
    if (query.dur == -1)
        register_operation(CLOSD, &query);
    else
        register_operation(IAMIN, &query);

    // destroy FIFO
    destroy_private_fifo(fd, fifo_name);

    return NULL;
}

void setup_thread_signals()
{
    struct sigaction action;
    action.sa_handler = sigpipe_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGPIPE, &action, NULL);
}

void sigpipe_handler(int signo)
{
    close(request_fd);
    unlink(private_fifo_name);
    pthread_exit(0);
}