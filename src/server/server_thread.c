#include "server_thread.h"

/**
 * @brief   The bathroom places. Each place holds the i value of the request
 */
long *bathroom;

long bathroom_places;

requests_holder *requests;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

void *answer_handler(void *arg)
{
    // read argument
    requests_holder rh = *(requests_holder *)arg;
    query request = rh.request;

    // get bathroom place
    pthread_mutex_lock(&mutex);
    long place = get_free_place();
    assign_place(place, &request);
    pthread_mutex_unlock(&mutex);

    register_operation(ENTER, &request);

    // get answer
    query answer = {request.i, getpid(), pthread_self(), request.dur, place + 1};

    // open private fifo
    char private_fifoname[MAX_FIFO_NAME_SIZE];
    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    // do the job
    usleep(request.dur * 1000);
    register_operation(TIMUP, &answer);

    // write the answer
    write(private_fifo_fd, &answer, sizeof(query));
    close(private_fifo_fd);

    // free bathroom
    pthread_mutex_lock(&mutex);
    bathroom[place] = EMPTY_BATHROOM_INDICATOR;
    pthread_mutex_unlock(&mutex);

    // free thread place
    pthread_mutex_lock(&server_mutex);
    requests[rh.place].place = EMPTY_THREAD_INDICATOR;
    pthread_mutex_unlock(&server_mutex);

    // notify semaphores
    sem_post(&sem_threads);
    sem_post(&sem_places);

    return NULL;
}

void *late_answer_handler(void *arg)
{
    // read argument
    requests_holder rh = *(requests_holder *)arg;
    query request = rh.request;

    // get answer
    query answer = {request.i, getpid(), pthread_self(), -1, -1};

    register_operation(TOOLATE, &answer);

    // open private fifo
    char private_fifoname[MAX_FIFO_NAME_SIZE];
    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname, &request);

    // write answer
    write(private_fifo_fd, &answer, sizeof(query));
    close(private_fifo_fd);

    // free thread place
    pthread_mutex_lock(&server_mutex);
    requests[rh.place].place = -1;
    pthread_mutex_unlock(&server_mutex);

    // notify semaphore
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
        *(places + i) = EMPTY_BATHROOM_INDICATOR;

    bathroom = places;
    bathroom_places = max_places;
}

void set_requests(requests_holder *rh)
{
    requests = rh;
}