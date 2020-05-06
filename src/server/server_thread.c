#include "server_thread.h"

/**
 * @brief   The bathroom places. Each place holds the i value of the request
 */
long bathroom_places[MAX_PLACES] = {0};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *answer_handler(void *arg)
{
    query request = *(query *)arg;

    // TODO return value check, 2nd part only

    pthread_mutex_lock(&mutex);

    long place = get_free_place();
    assign_place(place, &request);

    pthread_mutex_unlock(&mutex);

    register_operation(ENTER, &request);

    query answer = {request.i, getpid(), pthread_self(), request.dur, place};

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    usleep(request.dur * 1000);

    register_operation(TIMUP, &answer);

    write(private_fifo_fd, &answer, sizeof(query));

    close(private_fifo_fd);

    sem_post(&sem_threads);

    return NULL;
}

void *late_answer_handler(void *arg)
{
    query request = *(query *)arg;

    query answer = {request.i, getpid(), pthread_self(), -1, -1};

    register_operation(TOOLATE, &answer);

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    write(private_fifo_fd, &answer, sizeof(query));

    close(private_fifo_fd);

    return NULL;
}

long get_free_place()
{
    for (long i = 0; i < MAX_PLACES; ++i)
        if (bathroom_places[i] == 0)
            return i;
    return -1;
}

void assign_place(long place, query *query)
{
    bathroom_places[place] = query->i;
}

void destroy_mutex()
{
    pthread_mutex_destroy(&mutex);
}