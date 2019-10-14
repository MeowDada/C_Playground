.PHONY: all clean

CC 				     := gcc
CFLAGS               := -Wall -g -std=gnu99
LIB_DIR			     := ../lib

LOGGER_LIB_SRCS      := logger.c
LOGGER_LIB_TARGET    := liblogger.so

THPOOL_LIB_SRCS      := thpool.c
THPOOL_LIB_TARGET    := libthpool.so

COMMON_LIB_SRCS      := $(LOGGER_LIB_SRCS) $(THPOOL_LIB_SRCS)
COMMON_LIB_TARGET    := libcommon.so

THPOOL_TEST_PROG     := thpool_test
THPOOL_TEST_SRCS     := thpool_test.c
THPOOL_TEST_OBJS     := common

SERIALIZE_LIB_SRCS   := serialized_buffer.c serialize.c deserialize.c
SERIALIZE_LIB_TARGET := libserial.so

SERIALIZATION_TEST_PROG := serialization_test
SERIALIZATION_TEST_SRCS := serialization_test.c serialized_buffer.c serialize.c deserialize.c
SERIALIZATION_TEST_OBJS := $(patsubst %.c,%.o,$(SERIALIZATION_TEST_SRCS))

all: build_logger_lib build_thpool_lib build_common_lib build_serial_lib build_thpool_test build_serial_test

build_logger_lib:
	$(CC) -shared -o $(LIB_DIR)/$(LOGGER_LIB_TARGET) -fPIC $(LOGGER_LIB_SRCS)

build_thpool_lib:
	$(CC) -shared -o $(LIB_DIR)/$(THPOOL_LIB_TARGET) -fPIC $(THPOOL_LIB_SRCS)

build_common_lib:
	$(CC) -shared -o $(LIB_DIR)/$(COMMON_LIB_TARGET) -fPIC $(COMMON_LIB_SRCS)

build_serial_lib:
	$(CC) -shared -o $(LIB_DIR)/$(SERIALIZE_LIB_TARGET) -fPIC $(SERIALIZE_LIB_SRCS)

build_thpool_test:
	$(CC) -o $(THPOOL_TEST_PROG) $(THPOOL_TEST_SRCS) -L $(LIB_DIR) -l $(THPOOL_TEST_OBJS) -l pthread

build_serial_test:
	$(CC) $(CFLAGS) -c $(SERIALIZATION_TEST_SRCS)
	$(CC) $(CFLAGS) -o $(SERIALIZATION_TEST_PROG) $(SERIALIZATION_TEST_OBJS)

clean:
	rm -rf $(LIB_DIR)