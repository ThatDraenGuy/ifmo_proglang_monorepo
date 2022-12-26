CFLAGS     = -g -O2 -Wall -Werror -std=c17 -Wdiscarded-qualifiers -Wincompatible-pointer-types -Wint-conversion
CC         = gcc
LD         = gcc
ASM        = nasm
ASMFLAGS   =-felf64 -g

# Если есть код на C, то компоновку тоже нужно производить
# с помощью gcc, а не ld
task: task.o print_string.o print_file.o
	$(LD) -o $@ $^


task.o: task.c 
	$(CC) -c $(CFLAGS) -o $@ $<

print_string.o: print_string.asm
	$(ASM) $(ASMFLAGS) -o $@ $<
print_file.o: print_file.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

clean: 
	$(RM) hello hello.o string.o

.PHONY: clean

