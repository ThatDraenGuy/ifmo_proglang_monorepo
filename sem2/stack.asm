%include "print_hex.asm"

section .text
global _start


_start:
    call do_thing
exit:                        ; Это метка начала функции exit
    mov  rax, 60             ; Это функция exit
    xor  rdi, rdi            ; Это функция exit
    syscall                  ; Это функция exit



do_thing:
    push 0xaa
    push 0xbb
    push 0xff

    pop rdi
    call print_hex2
    pop rdi
    call print_hex2
    pop rdi
    call print_hex2
    ret