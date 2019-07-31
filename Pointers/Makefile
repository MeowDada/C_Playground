.PHONY: all clean

CC                := gcc
CCFLAGS           := 

INCLUDE_PATH				       := ../common
LIBRARY_PATH				       := ../lib

DOUBLE_POINTER_OPS_PROG 		   := double_pointer_ops
DOUBLE_POINTER_OPS_SRCS 		   := double_pointer_ops.c
DOUBLE_POINTER_OPS_DEPENDENCY      := logger

COPY_DATA_TO_POINTER_POSITION_PROG := copy_data_to_pointer_position
COPY_DATA_TO_POINTER_POSITION_SRCS := copy_data_to_pointer_position.c

CLEAN_TARGET				:= $(DOUBLE_POINTER_OPS_PROG) $(COPY_DATA_TO_POINTER_POSITION_PROG)

all: build_double_pointer_ops build_copy_data_to_pointer_position

build_double_pointer_ops:
	$(CC) $(CCFLAGS) -o $(DOUBLE_POINTER_OPS_PROG) $(DOUBLE_POINTER_OPS_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(DOUBLE_POINTER_OPS_DEPENDENCY)

build_copy_data_to_pointer_position:
	$(CC) $(CCFLAGS) -o $(COPY_DATA_TO_POINTER_POSITION_PROG) $(COPY_DATA_TO_POINTER_POSITION_SRCS)


clean:
	rm -rf $(CLEAN_TARGET)