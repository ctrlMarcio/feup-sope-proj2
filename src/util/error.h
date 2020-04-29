/**
 * @file    error.h
 * @brief   Defines the possible errors.
 * @date    2020-04-27
 */

#ifndef ERROR_H
#define ERROR_H

/**
 * @brief   Defines an error with the arguments
 */
#define ARGS_ERROR 1

/**
 * @brief   Defines an error connecting the client and the server
 */
#define CONNECTION_ERROR 2

/**
 * @brief   Defines an error while creating or opening a FIFO
 */
#define FIFO_CREATION_ERROR 3

/**
 * @brief   Defines an error when creating an alarm
 */
#define INIT_ALARM_ERROR 4

#endif