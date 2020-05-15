#include "server.h"
#include "server_fifo.c"
#include "server_thread.c"
#include "server_arguments.c"

/**
 * @brief   Flag that represents the state of the server listener
 */
bool running = true;

/**
 * @brief   The file descriptor of the public fifo
 */
int public_fifo_fd;

/**
 * @brief   The name of the public fifo
 */
char fifoname[MAX_FIFO_NAME_SIZE] = {0};

/**
 * @brief   The temporary sigint action used in the init_sigint()
 */
struct sigaction old_action;

int main(int argc, char *argv[])
{
    // Argument reading
    int number_of_seconds, number_of_places, number_of_threads;
    read_arguments(argc, argv, &number_of_seconds, &number_of_places, &number_of_threads, fifoname);

    // Initializes alarm handler
    init_sigint();
    init_alarm();

    // Initializes the bathroom array
    long places[number_of_places];
    set_bathroom_info(places, number_of_places);

    // Initializes the request holder array (required to manage threads)
    requests_holder requests_info[number_of_threads];
    for (int i = 0; i < number_of_threads; ++i)
        requests_info[i].place = EMPTY_THREAD_INDICATOR;
    set_requests(requests_info);

    // Starts the alarm
    alarm(number_of_seconds);

    // Initializes the threads and bathrooms semaphores
    sem_init(&sem_threads, 0, number_of_threads);
    sem_init(&sem_places, 0, number_of_places);

    // Opens the public FIFO
    public_fifo_fd = init_public_fifo(fifoname);

    // Reads requests in the main timeframe
    read_requests(requests_info, number_of_threads);

    // Reads the late requests
    read_late_requests(requests_info, number_of_threads);

    // Closes and destroys all the resources
    sem_destroy(&sem_threads);
    destroy_mutex();
    close(public_fifo_fd);
    unlink(fifoname);
    pthread_exit(0);
}

void read_requests(requests_holder requests_info[], int number_of_threads)
{
    while (running && !sem_wait(&sem_threads))
    {
        long index = get_free_thread_place(requests_info, number_of_threads);
        sem_wait(&sem_places);

        // If the user did not sent anything does not create threads
        if (read(public_fifo_fd, &(requests_info[index].request), sizeof(query)) <= 0)
        {
            sem_post(&sem_threads);
            sem_post(&sem_places);
            pthread_mutex_unlock(&server_mutex);
            continue;
        }

        requests_info[index].place = index;

        register_operation(RECVD, &(requests_info[index].request));
        pthread_t tid;
        pthread_create(&tid, NULL, answer_handler, &(requests_info[index]));
        pthread_detach(tid);
    }
}

void read_late_requests(requests_holder requests_info[], int number_of_threads)
{
    while (!sem_wait(&sem_threads))
    {
        long index = get_free_thread_place(requests, number_of_threads);

        if (read(public_fifo_fd, &(requests_info[index].request), sizeof(query)) <= 0)
            break;

        requests_info[index].place = index;

        register_operation(RECVD, &(requests_info[index].request));
        pthread_t tid;
        pthread_create(&tid, NULL, late_answer_handler, &(requests_info[index]));
        pthread_detach(tid);
    }
}

void init_alarm()
{
    struct sigaction action;
    action.sa_handler = terminate;
    sigfillset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGALRM, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install alarm handler\n");
        exit(INIT_ALARM_ERROR);
    }
}

void terminate(int signo)
{
    running = false;
    unlink(fifoname);
}

void init_sigint()
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigint_handler;
    sigaction(SIGINT, &action, &old_action);
}

void sigint_handler(int signo)
{
    sigaction(SIGINT, &old_action, NULL);
    destroy_mutex();
    close(public_fifo_fd);
    unlink(fifoname);
    kill(0, SIGINT);
}

long get_free_thread_place(requests_holder *requests, unsigned long number_of_threads)
{
    for (long i = 0; i < number_of_threads; ++i)
        if (requests[i].place == EMPTY_THREAD_INDICATOR)
            return i;
    return -1;
}