%define    WRITE   1

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    ; rdi: string pointer
    mov rax, -1
.loop:
    inc rax
    test byte [rdi+rax], 0xff   ;check if byte is null-terminator
    jne .loop
    ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    ; rdi: string pointer
    push    rdi             ;save caller-saved register
    call    string_length   ;string_length -> rax
    pop     rsi             ;string pointer (rdi -> rsi)
    mov     rdi, 1          ;stdout descriptor
    mov     rdx, rax        ;string length
    mov     rax,WRITE
    syscall
    ret