CC=gcc


build:	printer.o
	$(CC) -o printer $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
