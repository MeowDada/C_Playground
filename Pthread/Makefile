.PHONY: all clean

CC                := gcc

INCLUDE_PATH				:= ../common
LIBRARY_PATH				:= ../lib

PRODUCER_CONSUMER_PROG 		:= producer_consumer
PRODUCER_CONSUMER_SRCS 		:= producer_consumer.c
PRODUCER_CONSUMER_DEPENDENCY := logger

CLEAN_TARGET				:= $(PRODUCER_CONSUMER_PROG)

all: build_producer_consumer

build_producer_consumer:
	$(CC) -o $(PRODUCER_CONSUMER_PROG) $(PRODUCER_CONSUMER_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(PRODUCER_CONSUMER_DEPENDENCY)

clean:
	rm -rf $(CLEAN_TARGET)