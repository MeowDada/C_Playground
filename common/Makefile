.PHONY: all clean

CC 				     := gcc
LIB_DIR			     := ../lib

LOGGER_LIB_SRCS      := logger.c
LOGGER_LIB_TARGET    := liblogger.so

THPOOL_LIB_SRCS      := thpool.c
THPOOL_LIB_TARGET    := libthpool.so

all: build_logger_lib build_thpool_lib

build_logger_lib:
	$(CC) -shared -o $(LIB_DIR)/$(LOGGER_LIB_TARGET) -fPIC $(LOGGER_LIB_SRCS)

build_thpool_lib:
	$(CC) -shared -o $(LIB_DIR)/$(THPOOL_LIB_TARGET) -fPIC $(THPOOL_LIB_SRCS)

clean:
	rm -rf $(LIB_DIR)