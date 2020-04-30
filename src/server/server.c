#include "server.h"
#include "server_fifo.c"
#include "server_thread.c"
#include "server_arguments.c"

bool running = 1;

int public_fifo_fd;

char fifoname[MAX_FIFO_NAME_SIZE];

int main(int argc, char *argv[])
{
    int number_of_seconds;

    read_arguments(argc, argv, &number_of_seconds, fifoname);

    init_alarm();
    alarm(number_of_seconds);

    public_fifo_fd = init_public_fifo(fifoname);

    query request;
    while (running)
    {
        read(public_fifo_fd, &request, sizeof(query));

        // TODO: RECVD
        pthread_t tid;
        pthread_create(&tid, NULL, answer_handler, &request);
        pthread_detach(tid);
    }

    pthread_exit(0);
}

void terminate(int signo)
{
    running = false;
    close_public_fifo(public_fifo_fd, fifoname);
}

void init_alarm()
{
    struct sigaction action;
    action.sa_handler = &terminate;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGALRM, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install alarm handler\n");
        exit(INIT_ALARM_ERROR);
    }
}