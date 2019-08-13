.PHONY: all clean

CC 				     := gcc
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

all: build_logger_lib build_thpool_lib build_common_lib build_thpool_test

build_logger_lib:
	$(CC) -shared -o $(LIB_DIR)/$(LOGGER_LIB_TARGET) -fPIC $(LOGGER_LIB_SRCS)

build_thpool_lib:
	$(CC) -shared -o $(LIB_DIR)/$(THPOOL_LIB_TARGET) -fPIC $(THPOOL_LIB_SRCS)

build_common_lib:
	$(CC) -shared -o $(LIB_DIR)/$(COMMON_LIB_TARGET) -fPIC $(COMMON_LIB_SRCS)

build_thpool_test:
	$(CC) -o $(THPOOL_TEST_PROG) $(THPOOL_TEST_SRCS) -L $(LIB_DIR) -l $(THPOOL_TEST_OBJS) -l pthread

clean:
	rm -rf $(LIB_DIR)