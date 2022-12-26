
nasm -felf64 $1.asm -o bin/$1.o -g
ld -o run/$1 bin/$1.o
./run/$1