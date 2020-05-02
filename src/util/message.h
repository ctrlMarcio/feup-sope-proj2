/**
 * @file    message.h  
 * @brief   Standard output and error message handling.
 * @date    2020-05-02
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>
#include "utils.h"

/**
 * @brief   The clients does a request.
 */
#define IWANT "IWANT"

/**
 * @brief   The server receives the request.
 */
#define RECVD "RECVD"

/**
 * @brief   The server accepts a request.
 */
#define ENTER "ENTER"

/**
 * @brief   The client receives the request answer.
 */
#define IAMIN "IAMIN"

/**
 * @brief   The server finishes processing a request.
 */
#define TIMUP "TIMUP"

/**
 * @brief   The server receives a request too late.
 */
#define TOOLATE "2LATE"

/**
 * @brief   The client does a request oo late.
 */
#define CLOSD "CLOSD"

/**
 * @brief   The client cannot communicate with the server.
 */
#define FAILD "FAILD"

/**
 * @brief   The server cannot communicate with the client.
 */
#define GAVUP "GAVUP"

/**
 * @brief   The max size of the message.
 */
#define MAX_MESSAGE_SIZE 4096

/**
 * @brief Registers an operation to the standard output.
 * 
 * @param op_name   the operation
 * @param query     the query
 */
void register_operation(char *op_name, query *query);

#endif