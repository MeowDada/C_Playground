CC:=gcc
SRC:=$(wildcard *.c)
ORI:=$(basename $(SRC))
TAR:=$(patsubst %.c,%.o,$(SRC))
EXCEPT:=condition_variables hello_pthread mutex join
NAME:=$(filter-out $(EXCEPT), $(ORI))

all:$(NAME)

$(NAME): %: %.c
ifneq ($(EXCEPT), $@)
	$(CC) -o $@.o $<
endif

clean:
	rm -rf *.o
