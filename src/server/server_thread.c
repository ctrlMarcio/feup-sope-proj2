#include "server_thread.h"

/**
 * @brief   The bathroom places. Each place holds the i value of the request
 */
long *bathroom;

long bathroom_places;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

void *answer_handler(void *arg)
{
    query old_request = *(query *)arg;
    query request = {old_request.i, old_request.pid, old_request.tid, old_request.dur, old_request.pl};

    pthread_mutex_unlock(&server_mutex);

    pthread_mutex_lock(&mutex);

    long place = get_free_place();
    assign_place(place, &request);

    pthread_mutex_unlock(&mutex);

    register_operation(ENTER, &request);

    query answer = {request.i, getpid(), pthread_self(), request.dur, place + 1};

    char private_fifoname[MAX_FIFO_NAME_SIZE];
    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    usleep(request.dur * 1000);
    register_operation(TIMUP, &answer);
    write(private_fifo_fd, &answer, sizeof(query));
    close(private_fifo_fd);

    sem_post(&sem_threads);
    sem_post(&sem_places);

    bathroom[place] = 0;

    return NULL;
}

void *late_answer_handler(void *arg)
{
    query old_request = *(query *)arg;
    query request = {old_request.i, old_request.pid, old_request.tid, old_request.dur, old_request.pl};

    pthread_mutex_unlock(&server_mutex);

    query answer = {request.i, getpid(), pthread_self(), -1, -1};

    register_operation(TOOLATE, &answer);

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    write(private_fifo_fd, &answer, sizeof(query));
    close(private_fifo_fd);
    sem_post(&sem_threads);

    return NULL;
}

long get_free_place()
{
    for (long i = 0; i < bathroom_places; ++i)
        if (bathroom[i] == 0)
            return i;
    return -1;
}

void assign_place(long place, query *query)
{
    bathroom[place] = query->i;
}

void destroy_mutex()
{
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&server_mutex);
}

void set_bathroom_info(long *places, long max_places)
{
    for (int i = 0; i < max_places; ++i)
        *(places + i) = 0;

    bathroom = places;
    bathroom_places = max_places;
}