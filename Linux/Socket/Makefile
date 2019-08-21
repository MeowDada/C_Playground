.PHONY: all clean

CC                 := gcc
CFLAGS             := -Wall -g -std=gnu99

INCLUDE_PATH       := ../../common
LIBRARY_PATH       := ../../lib

TCP_CLIENT_PROG    := tcp_client
TCP_CLIENT_SRCS    := tcp_client.c
TCP_CLIENT_OBJS    := $(patsubst %.c,%.o,$(TCP_CLIENT_SRCS))

TCP_SERVER_PROG    := tcp_server
TCP_SERVER_SRCS    := tcp_server.c
TCP_SERVER_OBJS    := $(patsubst %.c,%.o,$(TCP_SERVER_SRCS))

TCP_STANDALONE_PROG := tcp_standalone
TCP_STANDALONE_SRCS := tcp_standalone.c
TCP_STANDALONE_OBJS := $(patsubst %.c,%.o,$(TCP_STANDALONE_SRCS))

all: build_tcp_client build_tcp_server build_tcp_standalone

build_tcp_client:
	$(CC) $(CFLAGS) -c $(TCP_CLIENT_SRCS) -I $(INCLUDE_PATH)
	$(CC) $(CFLAGS) -o $(TCP_CLIENT_PROG) $(TCP_CLIENT_OBJS) -L $(LIBRARY_PATH) -l logger

build_tcp_server:
	$(CC) $(CFLAGS) -c $(TCP_SERVER_SRCS) -I $(INCLUDE_PATH)
	$(CC) $(CFLAGS) -o $(TCP_SERVER_PROG) $(TCP_SERVER_OBJS) -L $(LIBRARY_PATH) -l logger

build_tcp_standalone:
	$(CC) $(CFLAGS) -c $(TCP_STANDALONE_SRCS) -I $(INCLUDE_PATH)
	$(CC) $(CFLAGS) -o $(TCP_STANDALONE_PROG) $(TCP_STANDALONE_OBJS) -L $(LIBRARY_PATH) -l common

clean:
	rm -rf $(TCP_CLIENT_PROG) $(TCP_SERVER_PROG) $(TCP_STANDALONE_PROG) $(wildcare *.o)