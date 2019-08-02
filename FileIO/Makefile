.PHONY: all clean

CC      := gcc
CCFLAGS := -D_GNU_SOURCE

PTR_STRUCT_FILE := ptr_struct_file
BUFF_TYPE_TEST   := buff_type_test

INCLUDE_PATH 		   := ../common
LIBRARY_PATH 		   := ../lib

FILE_POINTER_TEST_SRCS := file_pointer_test.c
FILE_POINTER_TEST_PROG := file_pointer_test
FILE_POINTER_TEST_OBJS := logger

all: build_ptr_struct_file build_buff_type_test build_file_pointer_test

build_ptr_struct_file:
	$(CC) -o $(PTR_STRUCT_FILE) $(PTR_STRUCT_FILE).c

build_buff_type_test:
	$(CC) -o $(BUFF_TYPE_TEST) $(BUFF_TYPE_TEST).c

build_file_pointer_test:
	$(CC) $(CCFLAGS) -o $(FILE_POINTER_TEST_PROG) $(FILE_POINTER_TEST_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(FILE_POINTER_TEST_OBJS)

clean: clean_ptr_struct_file clean_buff_type_test clean_file_pointer_test

clean_ptr_struct_file:
	rm -f $(PTR_STRUCT_FILE)

clean_buff_type_test:
	rm -f $(BUFF_TYPE_TEST)

clean_file_pointer_test:
	rm -f $(FILE_POINTER_TEST_PROG)
