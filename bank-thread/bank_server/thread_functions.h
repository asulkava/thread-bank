#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


int listenClient(int sock);


#endif
