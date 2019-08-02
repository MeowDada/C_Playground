.PHONY: all clean

CC                := gcc

INCLUDE_PATH				:= ../common
LIBRARY_PATH				:= ../lib

CONSOLE_TEMPLATE_PROG 		:= console_template
CONSOLE_TEMPLATE_SRCS 		:= console_template.c
CONSOLE_TEMPLATE_DEPENDENCY := logger

PROGRESS_BAR_PROG 	  		:= progress_bar
PROGRESS_BAR_SRCS 	  		:= progress_bar.c

CLEAN_TARGET				:= $(PROGRESS_BAR_PROG) $(CONSOLE_TEMPLATE_PROG)

build_progress_bar:
	$(CC) -o $(PROGRESS_BAR_PROG) $(PROGRESS_BAR_SRCS)

build_console_template:
	$(CC) -o $(CONSOLE_TEMPLATE_PROG) $(CONSOLE_TEMPLATE_SRCS) -I $(INCLUDE_PATH) -L $(LIBRARY_PATH) -l $(CONSOLE_TEMPLATE_DEPENDENCY)

all: build_progress_bar build_console_template

clean:
	rm -rf $(CLEAN_TARGET)