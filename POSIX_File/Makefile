.PHONY: all run_all clean

CC := gcc

POSIX_CP_EXAMPLE_SRCS   := posix_cp_example.c
POSIX_CP_EXAMPLE_TARGET := posix_cp_example

CREATE_NO_WRITEFILE_SRCS   := create_no_writefile.c
CREATE_NO_WRITEFILE_TARGET := create_no_writefile
CREATE_NO_WRITEFILE_NAME   := no_writefile.txt

TARGETS := $(POSIX_CP_EXAMPLE_TARGET) $(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME)

all: build_posix_cp_example build_create_no_write_file

build_posix_cp_example:
	$(CC) -o $(POSIX_CP_EXAMPLE_TARGET) $(POSIX_CP_EXAMPLE_SRCS)

build_create_no_write_file:
	$(CC) -o $(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_SRCS)

run_all: run_create_no_writefile

run_create_no_writefile:
ifeq ($(shell test -e $(CREATE_NO_WRITEFILE_TARGET) && echo -n yes),yes)
	./$(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME)
	ls -l
else
	build_create_no_write_file
	./$(CREATE_NO_WRITEFILE_TARGET) $(CREATE_NO_WRITEFILE_NAME)
	ls -l
endif

clean:
	rm $(TARGETS)
