.PHONY: all clean

CC                			:= gcc
CFLAGS                      := -Wall -g

INCLUDE_PATH				:= ../common
LIBRARY_PATH				:= ../lib

CONSOLE_TEMPLATE_PROG 		:= console_template
CONSOLE_TEMPLATE_SRCS 		:= console_template.c
CONSOLE_TEMPLATE_OBJS       := $(patsubst %.c,%.o,$(CONSOLE_TEMPLATE_SRCS))
CONSOLE_TEMPLATE_LIBS       := logger

PROGRESS_BAR_PROG 	  		:= progress_bar
PROGRESS_BAR_SRCS 	  		:= progress_bar.c
PROGRESS_BAR_OBJS           := $(patsubst %.c,%.o,$(PROGRESS_BAR_SRCS))

CLEAN_TARGET				:= $(PROGRESS_BAR_PROG) $(CONSOLE_TEMPLATE_PROG)

all:\
build_progress_bar\
build_console_template

build_progress_bar:
	$(CC) $(CFLAGS) -c $(PROGRESS_BAR_SRCS)
	$(CC) $(CFLAGS) -o $(PROGRESS_BAR_PROG) $(PROGRESS_BAR_OBJS)

build_console_template:
	$(CC) $(CFLAGS) -c $(CONSOLE_TEMPLATE_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(CONSOLE_TEMPLATE_LIBS)
	$(CC) $(CFLAGS) -o $(CONSOLE_TEMPLATE_PROG) $(CONSOLE_TEMPLATE_OBJS) -L $(LIBRARY_PATH) -l $(CONSOLE_TEMPLATE_LIBS)

clean:
	rm -rf $(CLEAN_TARGET)