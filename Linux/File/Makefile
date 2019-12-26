.PHONY: all clean

CC:=gcc

SOURCE_DIR=./src
BINARY_DIR=./bin

FMEM_OPEN_PROG=$(BINARY_DIR)/fmemopen
FMEM_OPEN_SRCS=$(addprefix $(SOURCE_DIR)/, fmemopen.c)

LSEEK_PROG=$(BINARY_DIR)/lseek
LSEEK_SRCS=$(addprefix $(SOURCE_DIR)/, lseek.c)

all: build_fmemopen build_lseek

build_fmemopen:
	@echo "building fmemopen..."
	@$(CC) -o $(FMEM_OPEN_PROG) $(FMEM_OPEN_SRCS)

build_lseek:
	@echo "building lseek..."
	@$(CC) -D_GNU_SOURCE -o $(LSEEK_PROG) $(LSEEK_SRCS)

clean:
	rm -f $(FMEM_OPEN_PROG)
	rm -f $(LSEEK_PROG)