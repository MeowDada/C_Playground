.PHONY: all clean

CC                  := gcc
CFLAGS              := -Wall -g

SYSLOG_EXAMPLE_PROG := syslog_example
SYSLOG_EXAMPLE_SRCS := $(SYSLOG_EXAMPLE_PROG).c
SYSLOG_EXAMPLE_OBJS := $(patsubst %.c,%.o,$(SYSLOG_EXAMPLE_SRCS))

all:%.o
	$(CC) $(CFLAGS) -o $(SYSLOG_EXAMPLE_PROG) $(SYSLOG_EXAMPLE_OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -rf $(SYSLOG_EXAMPLE_PROG)
