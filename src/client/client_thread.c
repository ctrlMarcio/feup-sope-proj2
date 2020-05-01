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

char fifoname2[1024];

void sigpipe_handler(int signo)
{
    unlink(fifoname2);
    pthread_exit(0);
}

void *request_handler(void *arg)
{
    struct sigaction action;
    action.sa_handler = sigpipe_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGPIPE, &action, NULL);

    // get values
    unsigned long i = *(int *)arg;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    int dur = rand() % MAX_DURATION;
    long pl = -1;

    // create FIFO
    char fifo_name[MAX_FIFO_NAME_SIZE];
    create_private_fifo(pid, tid, fifo_name);

    strcpy(fifoname2, fifo_name);

    // send request
    query query = {i, pid, tid, dur, pl};
    write(request_fd, &query, sizeof(query));
    register_operation(IWANT, &query);

    // open FIFO
    int fd = open_private_fifo(fifo_name);

    // receive response
    int bytes_read = read(fd, &query, sizeof(query));

    if (bytes_read <= 0)
    {
        register_operation(FAILD, &query);
        // destroy FIFO
        destroy_private_fifo(fd, fifo_name);
        return NULL;
    }

    register_operation(IAMIN, &query);

    // verify if failed
    if (query.dur == -1)
        register_operation(CLOSD, &query);

    // destroy FIFO
    destroy_private_fifo(fd, fifo_name);

    return NULL;
}