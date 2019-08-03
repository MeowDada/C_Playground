.PHONY: all clean

CC						  := gcc
CCFLAGS					  := 

INCLUDE_PATH			  := ../common
LIBRARY_PATH		      := ../lib

SIGNAL_TIMER_EXAMPLE_PROG := signal_timer_example
SIGNAL_TIMER_EXAMPLE_SRCS := signal_timer_example.c
SIGNAL_TIMER_EXAMPLE_OBJS := logger

all: build_signal_timer_example

build_signal_timer_example:
	$(CC) $(CCFLAGS) -o $(SIGNAL_TIMER_EXAMPLE_PROG) $(SIGNAL_TIMER_EXAMPLE_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(SIGNAL_TIMER_EXAMPLE_OBJS)

clean: clean_signal_timer_example

clean_signal_timer_example:
	rm -rf $(SIGNAL_TIMER_EXAMPLE_PROG)