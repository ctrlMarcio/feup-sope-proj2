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
    /*
     * Argument and alarm handling and FIFO creation
     */
    int number_of_seconds, number_of_places, number_of_threads;

    read_arguments(argc, argv, &number_of_seconds, &number_of_places, &number_of_threads, fifoname);

    init_sigint();

    long places[number_of_places];

    set_bathroom_info(places, number_of_places);

    sem_init(&sem_threads, 0, number_of_threads);
    sem_init(&sem_places, 0, number_of_places);

    init_alarm();
    alarm(number_of_seconds);

    public_fifo_fd = init_public_fifo(fifoname);

    /*
     * Request handling
     */
    static query request[MAX_PLACES];
    int i = 0;
    while (running && !sem_wait(&sem_threads))
    {
        sem_wait(&sem_places);
        pthread_mutex_lock(&server_mutex);

        if (read(public_fifo_fd, &(request[i % number_of_places]), sizeof(query)) <= 0) {
            sem_post(&sem_threads);
            sem_post(&sem_places);
            pthread_mutex_unlock(&server_mutex);
            continue;
        }

        register_operation(RECVD, &(request[i % number_of_places]));
        pthread_t tid;
        pthread_create(&tid, NULL, answer_handler, &(request[i % number_of_places]));
        pthread_detach(tid);
        ++i;
    }

    /*
     * Too late request handling. Empties the FIFO buffer
     */
    while (!sem_wait(&sem_threads))
    {
        pthread_mutex_lock(&server_mutex);
        if (read(public_fifo_fd, &(request[i % number_of_places]), sizeof(query)) <= 0)
            break;

        register_operation(RECVD, &(request[i % number_of_places]));
        pthread_t tid;
        pthread_create(&tid, NULL, late_answer_handler, &(request[i % number_of_places]));
        pthread_detach(tid);
        ++i;
    }

    /*
     * Closes, finally, the public FIFO
     */
    sem_destroy(&sem_threads);
    destroy_mutex();
    close(public_fifo_fd);
    unlink(fifoname);
    pthread_exit(0);
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