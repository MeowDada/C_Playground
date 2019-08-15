.PHONY: all clean

CC      := gcc
CCFLAGS := -Wall -g

SIMPLE_DAEMON_PROG := simple_daemon
SIMPLE_DAEMON_SRCS := simple_daemon.c
SIMPLE_DAEMON_OBJS := $(patsubst %.c,%.o,$(SIMPLE_DAEMON_SRCS))

SUB_DIRS           := $(wildcard */ */*/)
SRCS_FILES         := $(wildcard $(addsuffix *.c,$(SUB_DIRS))) $(wildcard *.c)
OBJS_FILES         := $(patsubst %.c,%.o,$(SRCS_FILES))

all: $(SRCS_FILES)
	$(CC) $(CCFLAGS) -c $^
	$(CC) $(CCFLAGS) -o $(SIMPLE_DAEMON_PROG) $(SIMPLE_DAEMON_OBJS)

%.o:%.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean: $(SIMPLE_DAEMON_PROG)
	rm -rf $@
	rm -rf $(OBJS_FILES)