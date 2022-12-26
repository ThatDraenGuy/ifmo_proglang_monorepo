# тот же файл с переменными

ASM=nasm
ASMFLAGS=-f elf64
LD=ld

a.o: a.asm
    $(ASM) $(ASMFLAGS) -o $@ $<

# b.o зависит от a.o но также и транзитивно от a.asm. 
# Не нужно указывать транзитивные зависимости, make их понимает

b.o: b.asm a.o
    $(ASM) $(ASMFLAGS) -o $@ $<

c.o: c.asm a.o
    $(ASM) $(ASMFLAGS) -o $@ $<

program: a.o b.o c.o
    $(LD) -o $@ $^