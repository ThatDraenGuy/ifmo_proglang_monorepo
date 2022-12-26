ASM=nasm
ASM_FLAGS=-g -felf64
LD=ld

obj/%.o: %.asm
	$(ASM) $(ASM_FLAGS) -o $@ $<

out/main: obj/lib.o obj/hex_start.o
	$(LD) -o $@ $^
run: out/main
	./out/main

.PHONY: run