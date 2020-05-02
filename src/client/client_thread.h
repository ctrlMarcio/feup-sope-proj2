/**
 * @file    client_fifo.h
 * @brief   Handles the threads for the client.
 * @date    2020-04-30
 */
#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

/**
 * @brief   Saves the file descriptor of the public fifo.
 */
int request_fd;

/**
 * @brief   Sends a request to the server and waits for the response.
 * 
 * @param arg       Pointer to the request sequential number
 * @return void*    Returns a null pointer
 */
void *request_handler(void *arg);

#endif