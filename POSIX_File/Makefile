.PHONY: all run_all clean

CC 						   := gcc

COPY_FROM_FILE_FNAME 	   := lorem.txt
COPIED_FILE_FNAME    	   := lorem_copy.txt

POSIX_CP_EXAMPLE_SRCS      := posix_cp_example.c
POSIX_CP_EXAMPLE_TARGET    := posix_cp_example

CREATE_NO_WRITEFILE_SRCS   := create_no_writefile.c
CREATE_NO_WRITEFILE_TARGET := create_no_writefile
CREATE_NO_WRITEFILE_NAME   := no_writefile.txt

POSIX_OPEN_EXAMPLE_SRCS    := posix_open_example.c
POSIX_OPEN_EXAMPLE_TARGET  := posix_open_example

FCNTL_DUP_FD_SRCS		   := fcntl_dup_fd.c
FCNTL_DUP_FD_TARGET		   := fcntl_dup_fd

DUMP_GENERATE_TEXT_PREFIX  := tmp

TARGETS := $(POSIX_CP_EXAMPLE_TARGET) $(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME) $(POSIX_OPEN_EXAMPLE_TARGET) $(COPIED_FILE_FNAME) 

all: build_posix_cp_example build_create_no_write_file build_posix_open_example build_posix_fcntl_dup_fd

build_posix_cp_example:
	$(CC) -o $(POSIX_CP_EXAMPLE_TARGET) $(POSIX_CP_EXAMPLE_SRCS)

build_create_no_write_file:
	$(CC) -o $(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_SRCS)

build_posix_open_example:
	$(CC) -o $(POSIX_OPEN_EXAMPLE_TARGET) $(POSIX_OPEN_EXAMPLE_SRCS)

build_posix_fcntl_dup_fd:
	$(CC) -o $(FCNTL_DUP_FD_TARGET) $(FCNTL_DUP_FD_SRCS)

run_all: run_posix_cp_example run_create_no_writefile run_posix_open_example

run_posix_cp_example:
ifeq ($(shell test -e $(POSIX_CP_EXAMPLE_TARGET) && echo -n yes),yes)
	./$(POSIX_CP_EXAMPLE_TARGET) $(COPY_FROM_FILE_FNAME) $(COPIED_FILE_FNAME)
	ls -l $(COPY_FROM_FILE_FNAME)
else
	build_posix_cp_example
	./$(POSIX_CP_EXAMPLE_TARGET) $(COPY_FROM_FILE_FNAME) $(COPIED_FILE_FNAME)
	ls -l $(COPY_FROM_FILE_FNAME)
endif

run_create_no_writefile:
ifeq ($(shell test -e $(CREATE_NO_WRITEFILE_TARGET) && echo -n yes),yes)
	./$(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME)
	ls -l
else
	build_create_no_write_file
	./$(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME)
	ls -l
endif

run_posix_open_example:
ifeq ($(shell test -e $(POSIX_OPEN_EXAMPLE_TARGET) && echo -n yes),yes)
	./$(POSIX_OPEN_EXAMPLE_TARGET)
	ls -l
else
	build_posix_open_example
	./$(POSIX_OPEN_EXAMPLE_TARGET)
	ls -l
endif

clean:
	rm -f $(TARGETS)
	rm -f $(DUMP_GENERATE_TEXT_PREFIX)*
