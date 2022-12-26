%include "print_string.asm"

section .data
codes:
    db      '0123456789ABCDEF'

section .text
global _start
_start:
    mov rdi, 0xF0043
    call print_hex
    mov rdi, 0xF0043
    call print_hex2

    mov rax, 60
    xor rdi,rdi
    syscall

print_hex:
    ; rdi - hex
    mov rcx, 64
    mov r8, rdi
    mov rdi, 1
    mov rdx, 1
    mov rax, 1
.loop:
    push r8
    sub rcx, 4
    sar r8, cl
    and r8, 0xf
    lea rsi, [codes + r8]

    push rcx
    syscall
    pop rcx
    pop r8
    test rcx, rcx
    jnz .loop
.end:
    ret

print_hex2:
    ;rdi: uint
    push word 0             ;null-terminator for a string
    mov rax, rdi            ;move uint to rax
    mov r8, rsp             ;save current stack value (address of null-terminator)
.loop:
    dec rsp                 ;using rsp as a pointer to where in a string next digit goes (going decrementally)
    mov rdx, rax
    and rdx, 0xf
    add rdx, codes
    mov dl, [rdx]
    mov [rsp], dl           ;save digit to string
    sar rax, 4
    test rax,rax            ;continue division or exit
    jne .loop
.end:
    mov rdi, rsp            ;move address of first digit to rdi to serve as an argument for print_string
    push r8
    call print_string       ;do print and not forget the caller-saved register
    pop r8
    mov rsp, r8             ;de-allocate the memory
    pop di                  ;remove null-ender
    ret