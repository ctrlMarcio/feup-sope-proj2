#include "server_thread.h"

void *answer_handler(void *arg)
{
    query request = *(query *)arg;
    
    query answer = {request.i, getpid(), pthread_self(), request.dur, 1};

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname);

    // todo check if thread is possible to execute

    usleep(request.dur * 1000);

    // todo change the place number

    write(private_fifo_fd, &answer, sizeof(query));
    printf("sent %ld\n", request.i);

    close(private_fifo_fd);

    return NULL;
}