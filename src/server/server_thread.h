/**
 * @file    server_thread.h
 * @brief   Methods related to thread handling.
 * @date    2020-05-02
 */
#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>

#include "server_fifo.h"
#include "../util/error.h"
#include "../util/utils.h"
#include "../util/message.c"

/**
 * @brief   Semaphore responsible for handling the thread number.
 */
sem_t sem_threads;

/**
 * @brief   Semaphore responsible for handling the bathroom places number.
 */
sem_t sem_places;

/**
 * @brief       Handles the client requests.
 * @warning     returns NULL
 * 
 * @param arg       the request pointer
 * @return void*    NULL
 */
void *answer_handler(void *arg);

/**
 * @brief       Handles the client too late requests.
 * @warning     returns NULL
 * 
 * @param arg       the request pointer
 * @return void*    NULL
 */
void *late_answer_handler(void *arg);

/**
 * @brief   Get the first free place in the bathroom.
 * 
 * @return long     the place index
 */
long get_free_place();

/**
 * @brief  Assigns a place to a request. 
 * 
 * @param place     the place index
 * @param query     the request
 */
void assign_place(long place, query *query);

/**
 * @brief   Destroys a mutex.
 */
void destroy_mutex();

/**
 * @brief   Sets up the bathroom information struct.
 * 
 * @param places        the bathroom places array pointer
 * @param max_places    the max places of the bathroom
 */
void set_bathroom_info(long *places, long max_places);

#endif