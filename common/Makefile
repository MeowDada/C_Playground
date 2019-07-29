.PHONY: all clean

CC 				     := gcc
LIB_DIR			     := ../lib

LOGGER_LIB_SRCS      := logger.c
LOGGER_LIB_TARGET    := liblogger.so

BUILD_LIB_TARGET     := $(LOGGER_LIB_TARGET)

all: build_logger_lib

build_logger_lib:
	$(CC) -shared -o $(LIB_DIR)/$(LOGGER_LIB_TARGET) -fPIC $(LOGGER_LIB_SRCS)

clean:
	rm -rf $(LIB_DIR)