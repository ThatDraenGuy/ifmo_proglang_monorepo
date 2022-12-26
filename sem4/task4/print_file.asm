; hello_mmap.asm
%define O_RDONLY 0 
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2
%define SYS_WRITE 1
%define SYS_OPEN 2
%define SYS_MMAP 9
%define FD_STDOUT 1


%define SYS_CLOSE 3
%define SYS_MUNMAP 11


section .text
global print_file

; use exit system call to shut down correctly
exit:
    mov  rax, 60
    xor  rdi, rdi
    syscall

; These functions are used to print a null terminated string
; rdi holds a string pointer
print_string:
    push rdi
    call string_length
    pop  rsi
    mov  rdx, rax 
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

string_length:
    xor  rax, rax
.loop:
    cmp  byte [rdi+rax], 0
    je   .end 
    inc  rax
    jmp .loop 
.end:
    ret

; This function is used to print a substring with given length
; rdi holds a string pointer
; rsi holds a substring length
print_substring:
    mov  rdx, rsi 
    mov  rsi, rdi
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

print_file:
    push r12
    ; Вызовите open и откройте fname в режиме read only.
    mov  rax, SYS_OPEN
    ;rdi - file_name
    mov  rsi, O_RDONLY    ; Open file read only
    mov  rdx, 0 	  ; We are not creating a file
                          ; so this argument has no meaning
    syscall
    ; rax holds the opened file descriptor now
    push rax

    %define SYS_FSTAT 5
    %define STRUCT_STAT_SIZE 144
    %define OFFSET_TO_ST_SIZE 48
    %define ST_SIZE_SIZE 8
    mov r12, rsp
    sub rsp, STRUCT_STAT_SIZE

    ; fstat call
    mov rdi, rax
    mov rax, SYS_FSTAT
    mov rsi, rsp
    syscall

    ;get file size to r12
    add rsp, OFFSET_TO_ST_SIZE
    mov r11, [rsp]
    mov rsp, r12

    pop rax

    push r11            ;push length
    ; mmap call
    mov r8, rax         ;fd
    mov rax, SYS_MMAP   ;mmap
    xor rdi,rdi         ;NULL fo addr
    mov rsi, r11        ;file length
    mov rdx, PROT_READ  
    mov r10, MAP_PRIVATE
    xor r9,r9           ;no offset
    syscall

    ; с помощью print_string теперь можно вывести его содержимое
    mov rdi,rax
    pop rsi     ;pop length
    push r8     ;push fd
    push rsi    ;push length
    push rax    ;push addr
    call print_substring

    ;UNMAP
    mov rax, SYS_MUNMAP
    pop rdi     ;pop addr
    pop rsi     ;pop length
    syscall

    ;CLOSE FILE
    mov rax, SYS_CLOSE
    pop r8      ;pop fd
    syscall

    pop r12
    ret