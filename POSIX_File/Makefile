.PHONY: all run_all clean

include Makefile.env

CC 			 := gcc

all: build_posix_cp_example build_create_no_writefile build_posix_open_example build_posix_fcntl_dup_fd

build_posix_cp_example:
	$(CC) -o $(POSIX_CP_EXAMPLE_TARGET) $(POSIX_CP_EXAMPLE_SRCS)

build_create_no_writefile:
	$(CC) -o $(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_SRCS)

build_posix_open_example:
	$(CC) -o $(POSIX_OPEN_EXAMPLE_TARGET) $(POSIX_OPEN_EXAMPLE_SRCS)

build_posix_fcntl_dup_fd:
	$(CC) -o $(FCNTL_DUP_FD_TARGET) $(FCNTL_DUP_FD_SRCS)

clean:
	rm -f $(TARGETS)
	rm -f $(DUMP_GENERATE_TEXT_PREFIX)*
