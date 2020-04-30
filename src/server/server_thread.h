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

#include "../util/error.h"
#include "../util/utils.h"

void *answer_handler(void *arg);

#endif