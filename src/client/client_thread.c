#include "client_thread.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "client_fifo.h"
#include "../util/utils.h"

void *request_handler(void *arg)
{
    // get values
    unsigned long i = *(int *)arg;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    int dur = rand() % MAX_DURATION;
    long pl = -1;

    // create FIFO
    char fifo_name[MAX_FIFO_NAME_SIZE];
    create_private_fifo(pid, tid, fifo_name);

    // send request
    printf("sent: %ld\n", i); // TEST
    query query = {i, pid, tid, dur, pl};
    write(request_fd, &query, sizeof(query));

    // open FIFO
    int fd = open_private_fifo(fifo_name);

    // receive response
    read(fd, &query, sizeof(query));
    printf("received: %ld\n", i); // TEST

    // verify if failed
    if (query.dur == -1)
        printf("LATE\n"); // TODO change with log

    // destroy FIFO
    destroy_private_fifo(fd, fifo_name);

    return NULL;
}