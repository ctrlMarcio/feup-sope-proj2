#include "server_thread.h"

void *answer_handler(void *arg)
{
    query request = *(query *)arg;
    
    register_operation(ENTER, &request);

    // todo return value check
    //long place = get_free_place();

    query answer = {request.i, getpid(), pthread_self(), request.dur, 1};

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    // todo GAVUP might be here when fd < 0
    int private_fifo_fd = open_private_fifo(private_fifoname);

    usleep(request.dur * 1000);

    register_operation(TIMUP, &answer); // todo this needs to be discussed

    write(private_fifo_fd, &answer, sizeof(query));

    close(private_fifo_fd);

    return NULL;
}

void *late_answer_handler(void *arg)
{
    query request = *(query *)arg;

    query answer = {request.i, getpid(), pthread_self(), -1, -1};

    register_operation(TOOLATE, &answer);

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    // todo GAVUP might be here when fd < 0
    int private_fifo_fd = open_private_fifo(private_fifoname);

    write(private_fifo_fd, &answer, sizeof(query));

    close(private_fifo_fd);

    return NULL;
}