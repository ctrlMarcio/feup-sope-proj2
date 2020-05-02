/**
 * @file    client_arguments.h
 * @brief   Treats the client's arguments.
 * @date    2020-04-30
 */
#ifndef CLIENT_ARGUMENTS_H
#define CLIENT_ARGUMENTS_H

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

#endif