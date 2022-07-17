# Makefile for test.c

CC = gcc

# Avoid re-declaring variables in the same scope with -Wshadow
# Avoid turning floats into doubles with -Wdouble-promotion
CCFLAGS_ERRORS = -Wall -Wextra -Wshadow -Wdouble-promotion -Werror
CCFLAGS_OPT = -Os
# gdb and valgrind support respectively
CCFLAGS_DEBUG = -g3 -ggdb3

CCFLAGS = ${CCFLAGS_ERRORS} ${CCFLAGS_OPT} ${CCFLAGS_DEBUG}

OBJS = vector.o

all: vector test_vector

.PHONY: vector test

test_vector: test_vector.c
	$(CC) -o test test_vector.c $(OBJS) $(CCFLAGS)
	
vector: vector.c
	$(CC) -c vector.c $(CCFLAGS)