[bits 64]
LoadGDT:   
    lgdt [rdi]
    mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq
GLOBAL LoadGDT

disablePIC:
    mov al, 0xff
    out 0xa1, al
    out 0x21, al
GLOBAL disablePIC