#include "server.h"

int public_fifo_fd;

char fifoname[MAX_FIFO_NAME_SIZE];

int main(int argc, char *argv[])
{
    // Argument reading and checking
    int number_of_seconds;

    read_arguments(argc, argv, &number_of_seconds, fifoname);

    // Alarm
    init_alarm();
    alarm(number_of_seconds);

    // FIFO
    public_fifo_fd = init_public_fifo(fifoname);

    // Thread
    query request;

    while (1)
    {
        read(public_fifo_fd, &request, sizeof(query));

        // TODO: RECVD
        pthread_t tid;
        pthread_create(&tid, NULL, answer_handler, &request);
        pthread_detach(tid);
    }

    pthread_exit(0);
}

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname)
{
    if (argc != ARGUMENT_COUNT)
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(ARGS_ERROR);
    }

    *number_of_seconds = atoi(argv[2]);

    if (*number_of_seconds <= 0)
    {
        fprintf(stderr, "The number of seconds must be greater than 0\n");
        exit(ARGS_ERROR);
    }

    strcpy(fifoname, argv[ARGUMENT_COUNT - 1]);

    if (!strcmp(fifoname, ".") || !strcmp(fifoname, ".."))
    {
        fprintf(stderr, "The fifo name is invalid\n");
        exit(ARGS_ERROR);
    }
}

int init_public_fifo(char *fifoname)
{
    if (mkfifo(fifoname, FIFO_PERMS) < 0)
    {
        perror("Can't create FIFO");
        exit(FIFO_CREATION_ERROR);
    }
    else
    {
        printf("FIFO '%s' successfully created\n", fifoname);
    }

    int fd;

    if ((fd = open(fifoname, O_RDONLY)) != -1)
        printf("FIFO '%s' [%d] openned in READONLY mode\n", fifoname, fd);
    else
    {
        perror("Could not open FIFO");
        exit(FIFO_CREATION_ERROR);
    }

    return fd;
}

void close_public_fifo()
{
    close(public_fifo_fd);
    unlink(fifoname);
}

void terminate(int signo)
{
    // todo close fifo and other stuff
    close_public_fifo();
    pthread_exit(0);
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

void *answer_handler(void *arg)
{
    query request = *(query *)arg;

    // todo check if thread is possible to execute

    usleep(request.dur * 1000);

    // todo change the place number
    query answer = {request.i, getpid(), pthread_self(), request.dur, 1};

    char private_fifoname[MAX_FIFO_NAME_SIZE];

    sprintf(private_fifoname, "/%s/%ld.%ld", FIFO_FOLDER, (long)request.pid, (long)request.tid);

    int private_fifo_fd = open_private_fifo(private_fifoname);

    write(private_fifo_fd, &answer, sizeof(query));
}

int open_private_fifo(char *fifo_name)
{
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Server unavailable, aborting\n");
        exit(CONNECTION_ERROR);
    }

    return fd;
}