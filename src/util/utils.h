/**
 * @brief Provides useful functions, macros and constants. 
 * 
 */
#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <pthread.h>

/**
 * @brief   The maximum size allowed for the name of the FIFO.
 */
#define MAX_FIFO_NAME_SIZE 1024

/**
 * @brief   The permissions of the FIFOs   
 */
#define FIFO_PERMS 0777

/**
 * @brief   The max duration of a request
 */
#define MAX_DURATION 2_000

/**
 * @brief   Temporary value of max slots in the bathroom
 */
#define MAX_PLACES 1073741824

/**
 * @brief   The name of the folder where FIFOs are to be interchanged.
 */
const char *FIFO_FOLDER = "/tmp/";

/**
 * @brief   The query struct. This struct is used in both request and answer operations
 */
typedef struct query
{
    /**
     * @brief   A sequencial number generated by the U
     */
    unsigned long i;

    /**
     * @brief   The process id
     */
    pid_t pid;

    /**
     * @brief   The thread id
     */
    pthread_t tid;

    /**
     * @brief   The use duration. -1 in the response if the server is going to end
     */
    int dur;

    /**
     * @brief   The place number of the request. -1 if cant occupy slot or the server is going to end
     */
    long pl;
} query;

#endif
