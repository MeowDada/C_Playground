.PHONY: all clean

CC := gcc
PTR_STRUCT_FILE := ptr_struct_file
BUFF_TYPE_TEST   := buff_type_test

all: all_ptr_struct_file all_buff_type_test

all_ptr_struct_file:
	$(CC) -o $(PTR_STRUCT_FILE) $(PTR_STRUCT_FILE).c

all_buff_type_test:
	$(CC) -o $(BUFF_TYPE_TEST) $(BUFF_TYPE_TEST).c

clean: clean_ptr_struct_file clean_buff_type_test

clean_ptr_struct_file:
	rm $(PTR_STRUCT_FILE)

clean_buff_type_test:
	rm $(BUFF_TYPE_TEST)
