CC=gcc


build:	generic_list.o generic_print.o
	$(CC) -o main $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
