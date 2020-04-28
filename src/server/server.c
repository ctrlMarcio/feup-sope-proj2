#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>

#include "server.h"

int public_fifo_fd;

int main(int argc, char *argv[])
{
    // Argument reading and checking
    int number_of_seconds;
    char fifoname[MAX_FIFO_NAME_SIZE];

    read_arguments(argc, argv, &number_of_seconds, fifoname);

    // Alarm
    init_alarm();
    alarm(number_of_seconds);

    // FIFO
    public_fifo_fd = init_public_fifo();

    // Thread
    while (1){
        // Read information struct and create thread
    }

    exit(0);
}

void read_arguments(int argc, char *argv[], int *number_of_seconds, char *fifoname)
{
    if (argc != ARGUMENT_COUNT)
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <number of seconds> <fifoname>\n", argv[0]);
        // todo replace the exit code
        exit(1);
    }

    *number_of_seconds = atoi(argv[2]);

    if (*number_of_seconds <= 0)
    {
        fprintf(stderr, "The number of seconds must be greater than 0\n");
        // todo replace the exit code
        exit(1);
    }

    strcpy(fifoname, argv[ARGUMENT_COUNT - 1]);

    if (!strcmp(fifoname, ".") || !strcmp(fifoname, ".."))
    {
        fprintf(stderr, "The fifo name is invalid\n");
        // todo replace the exit code
        exit(1);
    }
}

int init_public_fifo()
{
    if (mkfifo(PUBLIC_FIFO_NAME, FIFO_PERMS) < 0)
    {
        perror("Can't create FIFO");
        // todo replace the exit code
        exit(1);
    }
    else
    {
        printf("FIFO '%s' sucessfully created\n", PUBLIC_FIFO_NAME);
    }

    int fd;

    if ((fd = open(PUBLIC_FIFO_NAME, O_RDONLY)) != -1)
        printf("FIFO '%s' [%d] openned in READONLY mode\n", PUBLIC_FIFO_NAME, fd);
    else
    {
        perror("Could not open FIFO");
        // todo replace the exit code
        exit(2);
    }

    return fd;
}

void close_public_fifo(){
    close(public_fifo_fd);
    unlink(PUBLIC_FIFO_NAME);
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
        // todo replace the exit code
        exit(1);
    }
}