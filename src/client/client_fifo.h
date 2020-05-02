/**
 * @file    client_fifo.h
 * @brief   Handles the client's FIFO's access.
 * @date    2020-04-30
 */
#ifndef CLIENT_FIFO_H
#define CLIENT_FIFO_H

#include <pthread.h>

/**
 * @brief   Opens the public FIFO.
 * 
 * @param fifo_name The name of the FIFO
 * @return int      The FIFO file descriptor
 */
int open_public_fifo(char *fifo_name);

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


#endif