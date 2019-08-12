.PHONY: all clean

CC                := gcc
CCFLAGS           := -pthread

INCLUDE_PATH				:= ../common
LIBRARY_PATH				:= ../lib

PRODUCER_CONSUMER_PROG 		:= producer_consumer
PRODUCER_CONSUMER_SRCS 		:= producer_consumer.c
PRODUCER_CONSUMER_DEPENDENCY := logger

MULTIPLE_PRODUCER_CONSUMER_PROG := multiple_producer_consumer
MULTIPLE_PRODUCER_CONSUMER_SRCS := multiple_producer_consumer.c
MULTIPLE_PRODUCER_CONSUMER_OBJS := logger

CLEAN_TARGET				:= $(PRODUCER_CONSUMER_PROG) $(MULTIPLE_PRODUCER_CONSUMER_PROG)

all: build_producer_consumer build_multiple_producer_consumer

build_producer_consumer:
	$(CC) $(CCFLAGS) -o $(PRODUCER_CONSUMER_PROG) $(PRODUCER_CONSUMER_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(PRODUCER_CONSUMER_DEPENDENCY)

build_multiple_producer_consumer:
	$(CC) $(CCFLAGS) -o $(MULTIPLE_PRODUCER_CONSUMER_PROG) $(MULTIPLE_PRODUCER_CONSUMER_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(MULTIPLE_PRODUCER_CONSUMER_OBJS)

clean:
	rm -rf $(CLEAN_TARGET)
	rm -rf *.log