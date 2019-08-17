.PHONY: all clean

CC			 		    := gcc
CFLAGS                  := -Wall -g

LIBRARY_PATH 		    := ../lib
INCLUDE_PATH		    := ../common

LINK_SO_TEST_SRCS       := link_so_test.c
LINK_SO_TEST_TARGET	    := link_so_test
LINK_SO_TEST_DEPENDENCY := logger

ARRAY_PTR_PROG          := array_ptr
ARRAY_PTR_SRCS          := array_ptr.c
ARRAY_PTR_OBJS          := $(patsubst %.c,%.o,$(ARRAY_PTR_SRCS))

FREE_PROG               := free
FREE_SRCS               := free.c
FREE_OBJS               := $(patsubst %.c,%.o,$(FREE_SRCS))

FUNC_ARG_PTR_PROG       := func_arg_ptr
FUNC_ARG_PTR_SRCS       := func_arg_ptr.c
FUNC_ARG_PTR_OBJS       := $(patsubst %.c,%.o,$(FUNC_ARG_PTR_SRCS))

HELLO_PTHREAD_PROG      := hello_pthread
HELLO_PTHREAD_SRCS      := hello_pthread.c
HELLO_PTHREAD_OBJS      := $(patsubst %.c,%.o,$(HELLO_PTHREAD_SRCS))

all: build_link_so_test build_array_ptr build_free build_func_arg_ptr build_hello_pthread

build_link_so_test:
	$(CC) $(CFLAGS) -o $(LINK_SO_TEST_TARGET) $(LINK_SO_TEST_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(LINK_SO_TEST_DEPENDENCY)

build_array_ptr:
	$(CC) $(CFLAGS) -c $(ARRAY_PTR_SRCS)
	$(CC) $(CFLAGS) -o $(ARRAY_PTR_PROG) $(ARRAY_PTR_OBJS)

build_free:
	$(CC) $(CFLAGS) -c $(FREE_SRCS)
	$(CC) $(CFLAGS) -o $(FREE_PROG) $(FREE_OBJS)

build_func_arg_ptr:
	$(CC) $(CFLAGS) -c $(FUNC_ARG_PTR_SRCS)
	$(CC) $(CFLAGS) -o $(FUNC_ARG_PTR_PROG) $(FUNC_ARG_PTR_OBJS)

build_hello_pthread:
	$(CC) $(CFLAGS) -c $(HELLO_PTHREAD_SRCS)
	$(CC) $(CFLAGS) -o $(HELLO_PTHREAD_PROG) $(HELLO_PTHREAD_OBJS) -lpthread

clean:
	rm -rf $(ARRAY_PTR_PROG) $(FREE_PROG) $(FUNC_ARG_PTR_PROG) $(HELLO_PTHREAD_PROG)
