#include "server.h"
#include "server_fifo.c"
#include "server_thread.c"
#include "server_arguments.c"

/**
 * @brief   Flag that represents the state of the server listener
 */
bool running = 1;

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
    // reads the arguments
    int number_of_seconds, number_of_places, number_of_threads;
    read_arguments(argc, argv, &number_of_seconds, &number_of_places, &number_of_threads, fifoname);

    // initializes the handlers
    init_sigint();
    init_alarm();

    // initializes bathroom array (manage bathrooms)
    long places[number_of_places];
    set_bathroom_info(places, number_of_places);

    // initialize request holder array (manage threads)
    requests_holder requests[number_of_threads];
    for (int i = 0; i < number_of_threads; ++i)
        requests[i].place = EMPTY_THREAD_INDICATOR;
    set_requests(requests);

    // starts counting the alarm
    alarm(number_of_seconds);

    // initializes the threads and bathrooms semaphores
    sem_init(&sem_threads, 0, number_of_threads);
    sem_init(&sem_places, 0, number_of_places);

    // opens the public fifo
    public_fifo_fd = init_public_fifo(fifoname);

    // reads requests in the main timeframe
    while (running && !sem_wait(&sem_threads))
    {
        long index = get_free_thread_place(requests, number_of_threads);
        sem_wait(&sem_places);

        // if the user didn't sent anything doesn't create threads
        if (read(public_fifo_fd, &(requests[index].request), sizeof(query)) <= 0)
        {
            sem_post(&sem_threads);
            sem_post(&sem_places);
            pthread_mutex_unlock(&server_mutex);
            continue;
        }

        requests[index].place = index;

        register_operation(RECVD, &(requests[index].request));
        pthread_t tid;
        pthread_create(&tid, NULL, answer_handler, &(requests[index]));
        pthread_detach(tid);
    }

    // reads the late requests
    read_late_requests(number_of_threads);

    // closes and destroys all the resources
    sem_destroy(&sem_threads);
    destroy_mutex();
    close(public_fifo_fd);
    unlink(fifoname);
    pthread_exit(0);
}

void read_late_requests(int number_of_threads)
{
    while (!sem_wait(&sem_threads))
    {
        long index = get_free_thread_place(requests, number_of_threads);

        if (read(public_fifo_fd, &(requests[index].request), sizeof(query)) <= 0)
            break;

        requests[index].place = index;

        register_operation(RECVD, &(requests[index].request));
        pthread_t tid;
        pthread_create(&tid, NULL, late_answer_handler, &(requests[index]));
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