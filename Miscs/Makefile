.PHONY: all clean

CC			 		    := gcc
LIBRARY_PATH 		    := ../lib
INCLUDE_PATH		    := ../common

LINK_SO_TEST_SRCS       := link_so_test.c
LINK_SO_TEST_TARGET	    := link_so_test
LINK_SO_TEST_DEPENDENCY := logger

all: build_link_so_test

build_link_so_test:
	$(CC) -o $(LINK_SO_TEST_TARGET) $(LINK_SO_TEST_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(LINK_SO_TEST_DEPENDENCY)

clean: clean_link_so_test

clean_link_so_test:
	rm $(LINK_SO_TEST_TARGET)
