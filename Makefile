.PHONY: all clean

MAKEFILES := $(shell find . -maxdepth 2 -type f -name Makefile)
SUBDIRS   := $(filter-out ./,$(dir $(MAKEFILES)))

MAKE_CMD  := all
CLEAN_CMD := clean

all:
	for dir in $(SUBDIRS) ; do \
		make -C $$dir $(MAKE_CMD); \
	done

clean:
	for dir in $(SUBDIRS) ; do   \
		make -C $$dir $(CLEAN_CMD); \
	done
