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

#include "server_fifo.h"
#include "../util/error.h"
#include "../util/utils.h"
#include "../util/message.c"

void *answer_handler(void *arg);

void *late_answer_handler(void *arg);

#endif