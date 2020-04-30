/**
 * @file    client.c
 * @brief   Runs the main client operations.
 * @date    2020-04-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "../util/error.h"
#include "../util/utils.h"

/**
 * @brief   Saves the file descriptor of the public fifo.
 */
int request_fd;

/**
 * @brief   The delay, in milliseconds, between each request thread.
 */
const int MILLIS_DELAY = 100;

/**
 * @brief   Reads the arguments passed by the terminal.
 * 
 * @param argc  The argument count
 * @param argv  The arguments array
 * @return int  The number of seconds for the program to run, "nsecs"
 */
int read_args(int argc, char *argv[]);

/**
 * @brief   Appends and updates the FIFO's name.
 * 
 * @param dst       The string to read the name to
 * @param fifo_name The name of the FIFO
 */
void read_fifo_name(char *dst, char *fifo_name);

/**
 * @brief   Sets up the SIGALARM.
 * 
 * @param nsecs     The number of seconds to set the alarm to
 */
void setup_alarm(int nsecs);

/**
 * @brief   Terminates the program.
 * @details Is called when SIGALARM is received. Can be called by any signal.
 * 
 * @param signo     The signal identification number
 */
void terminate(int signo);

/**
 * @brief   Opens the public FIFO.
 * 
 * @param fifo_name The name of the FIFO
 * @return int      The FIFO file descriptor
 */
int open_public_fifo(char *fifo_name);

/**
 * @brief   Sends a request to the server and waits for the response.
 * 
 * @param arg       Pointer to the request sequential number
 * @return void*    Returns a null pointer
 */
void *request_handler(void *arg);

/**
 * @brief   Creates a private FIFO.
 * 
 * @param pid       The process ID
 * @param tid       The thread ID
 * @param fifo_name The pointer to the name of the FIFO to save 
 */
void create_private_fifo(pid_t pid, pthread_t tid, char *fifo_name);

/**
 * @brief   Opens an existing private FIFO.
 * @warning The FIFO must already exist.
 * 
 * @param fifo_name The name of the FIFO
 * @return int      The file descriptor of the FIFO
 */
int open_private_fifo(char *fifo_name);

/**
 * @brief   Closes and destroys the private FIFO.
 * 
 * @param fd        The FIFO fd
 * @param name      The name of the FIFO
 */
void destroy_private_fifo(int fd, char *name);

int main(int argc, char *argv[])
{
    // read arguments
    int nsecs = read_args(argc, argv);

    // save public fifo name
    char public_fifo[MAX_FIFO_NAME_SIZE];
    read_fifo_name(public_fifo, argv[3]);

    // open public fifo
    request_fd = open_public_fifo(public_fifo);

    // send sigalarm
    setup_alarm(nsecs);

    // setup random seed
    srand(time(NULL));

    pthread_t thread;
    unsigned long request_count = 0;
    while (1)
    {
        if (pthread_create(&thread, NULL, request_handler, &request_count))
        {
            perror("Creating thread");
            exit(INIT_THREAD);
        }
        request_count++;
        pthread_detach(thread);
        usleep(MILLIS_DELAY * 1000); // usleep is in microseconds
    }

    // only for precaution, the regular termination of the program is in the function "terminate"
    pthread_exit(0);
}

int read_args(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s -t <nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    if (strcmp(argv[1], "-t"))
    {
        fprintf(stderr, "Usage: %s -t <nsecs> fifoname\n", argv[0]);
        exit(ARGS_ERROR);
    }

    // verifies if the nsecs arguments is ok
    int nsecs = atoi(argv[2]);
    if (nsecs <= 0)
    {
        fprintf(stderr, "Invalid nsecs\n");
        exit(ARGS_ERROR);
    }

    return nsecs;
}

void read_fifo_name(char *dst, char *fifo_name)
{
    if (!strcmp(fifo_name, "..") || !strcmp(fifo_name, "."))
    {
        fprintf(stderr, "Invalid FIFO name\n");
        exit(ARGS_ERROR);
    }

    strcpy(dst, FIFO_FOLDER);
    strcat(dst, fifo_name);
}

void setup_alarm(int nsecs)
{
    struct sigaction action;
    action.sa_handler = terminate;
    action.sa_flags = 0;
    sigfillset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);

    alarm(nsecs);
}

void terminate(int signo)
{
    close(request_fd);
    pthread_exit(0);
}

int open_public_fifo(char *fifo_name)
{
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1)
    {
        fprintf(stderr, "Server unavailable, aborting\n");
        exit(CONNECTION_ERROR);
    }

    return fd;
}

void *request_handler(void *arg)
{
    // get values
    unsigned long i = *(int *)arg;
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    int dur = rand() % MAX_DURATION;
    long pl = -1;

    // create FIFO
    char fifo_name[MAX_FIFO_NAME_SIZE];
    create_private_fifo(pid, tid, fifo_name);

    // send request
    printf("sent: %ld\n", i); // TEST
    query query = {i, pid, tid, dur, pl};
    write(request_fd, &query, sizeof(query));

    // open FIFO
    int fd = open_private_fifo(fifo_name);

    // receive response
    read(fd, &query, sizeof(query));
    printf("received: %ld\n", i); // TEST

    // verify if failed
    if (query.dur == -1)
        printf("LATE\n"); // TODO change with log

    // destroy FIFO
    destroy_private_fifo(fd, fifo_name);

    return NULL;
}

void create_private_fifo(pid_t pid, pthread_t tid, char *fifo_name)
{
    sprintf(fifo_name, "%s%d.%ld", FIFO_FOLDER, pid, tid);

    if (mkfifo(fifo_name, FIFO_PERMS) < 0)
    {
        perror("Create private FIFO");
        exit(FIFO_CREATION_ERROR);
    }
}

int open_private_fifo(char *fifo_name)
{
    int fd;

    if ((fd = open(fifo_name, O_RDONLY)) == -1)
    {
        perror("Open private FIFO");
        unlink(fifo_name);
        exit(FIFO_CREATION_ERROR);
    }

    return fd;
}

void destroy_private_fifo(int fd, char *name)
{
    close(fd);
    unlink(name);
}