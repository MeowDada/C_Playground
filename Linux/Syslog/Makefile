.PHONY: all clean

CC                  := gcc
CFLAGS              := -Wall -g

SYSLOG_EXAMPLE_PROG := syslog_example
SYSLOG_EXAMPLE_SRCS := $(SYSLOG_EXAMPLE_PROG).c
SYSLOG_EXAMPLE_OBJS := $(patsubst %.c,%.o,$(SYSLOG_EXAMPLE_SRCS))

all:
	build_syslog_example

build_syslog_example: $(SYSLOG_EXAMPLE_SRCS)
	$(CC) $(CFLAGS) -c $^
	$(CC) $(CFLAGS) -o $(SYSLOG_EXAMPLE_PROG) $(SYSLOG_EXAMPLE_OBJS)


clean:
	rm -rf $(SYSLOG_EXAMPLE_PROG)
