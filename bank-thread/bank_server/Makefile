
PROGS = server commands thread_functions
CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99 -D_POSIX_SOURCE
LDFLAGS = -lpthread -pthread

all: ${PROGS}

commands: commands.c
	${CC} ${CFLAGS} -fPIC -c commands.c
	
thread_functions: thread_functions.c
	${CC} ${CFLAGS} -fPIC -c thread_functions.c

clean:
	rm  thread_functions.o
	rm  commands.o
	rm  -f ${PROGS} *~
