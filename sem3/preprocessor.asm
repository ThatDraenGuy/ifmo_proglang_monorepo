%define i 1
     
%xdefine d i * 3
     
mov rax, d
     
; let's redefine i
%define i 100
mov rax, d

%define process(x) (x * 3 + 23)
     
mov rax,  process( 9 )