CC=gcc


build:	printf.o
	$(CC) -o printf $^

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
