.PHONY: all clean

MAKEFILES = $(shell find . -type f -name Makefile)
SUBDIRS =   $(filter-out ./,$(dir $(MAKEFILES)))

all:
	for dir in $(SUBDIR) ; do \
		make -C $$dir all; \
	done

clean:
	
