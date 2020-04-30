#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>
#include "utils.h"

#define IWANT "IWANT"
#define RECVD "RECVD"
#define ENTER "ENTER"
#define IAMIN "IAMIN"
#define TIMUP "TIMUP"
#define TOOLATE "2LATE"
#define CLOSD "CLOSD"
#define FAILD "FAILD"
#define GAVUP "GAVUP"

#define MAX_MESSAGE_SIZE 4096

void register_operation(char *op_name, query *query);

#endif