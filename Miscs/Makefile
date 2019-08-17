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

MUTEX_EXAMPLE_PROG      := mutex_example
MUTEX_EXAMPLE_SRCS      := mutex_example.c
MUTEX_EXAMPLE_OBJS      := $(patsubst %.c,%.o,$(MUTEX_EXAMPLE_SRCS))

PTHREAD_JOIN_EXAMPLE_PROG := pthread_join_example
PTHREAD_JOIN_EXAMPLE_SRCS := pthread_join_example.c
PTHREAD_JOIN_EXAMPLE_OBJS := $(patsubst %.c,%.o,$(PTHREAD_JOIN_EXAMPLE_SRCS))

CONDITION_VARIABLES_PROG  := condition_variables
CONDITION_VARIABLES_SRCS  := condition_variables.c
CONDITION_VARIABLES_OBJS  := $(patsubst %.c,%.o,$(CONDITION_VARIABLES_SRCS))

all: build_link_so_test build_array_ptr build_free build_func_arg_ptr build_hello_pthread build_mutex_example build_join_example \
build_pthread_join_example build_condition_variables

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

build_mutex_example:
	$(CC) $(CFLAGS) -c $(MUTEX_EXAMPLE_SRCS)
	$(CC) $(CFLAGS) -o $(MUTEX_EXAMPLE_PROG) $(MUTEX_EXAMPLE_OBJS) -lpthread

build_pthread_join_example:
	$(CC) $(CFLAGS) -c $(PTHREAD_JOIN_EXAMPLE_SRCS)
	$(CC) $(CFLAGS) -o $(PTHREAD_JOIN_EXAMPLE_PROG) $(PTHREAD_JOIN_EXAMPLE_OBJS) -lpthread

build condition_variables:
	$(CC) $(CFLAGS) -c $(CONDITION_VARIABLES_SRCS)
	$(CC) $(CFLAGS) -o $(CONDITION_VARIABLES_PROG) $(CONDITION_VARIABLES_OBJS) -lpthread

clean:
	rm -rf $(ARRAY_PTR_PROG) $(FREE_PROG) $(FUNC_ARG_PTR_PROG) $(HELLO_PTHREAD_PROG) $(MUTEX_EXAMPLE_PROG) $(PTHREAD_JOIN_EXAMPLE_PROG)
