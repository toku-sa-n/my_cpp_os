[bits 32]
    global io_hlt
    global io_cli
    global io_sti
    global io_stihlt

    global io_in8
    global io_in16
    global io_in32

    global io_out8
    global io_out16
    global io_out32

    global io_load_eflags
    global io_store_eflags

    global load_gdtr
    global load_idtr

    global asm_interrupt_handler_21
    global asm_interrupt_handler_27
    global asm_interrupt_handler_2c

    extern interrupt_handler_21
    extern interrupt_handler_27
    extern interrupt_handler_2c

[section .text]

io_hlt:
    hlt
    ret

io_cli:
    cli
    ret

io_sti:
    sti
    ret

io_stihlt:
    sti
    hlt
    ret

; int io_in(8|16|32)(int port);
io_in8:
    mov edx,[esp+4]
    mov eax,0
    in al,dx
    ret

io_in16:
    mov edx,[esp+4]
    mov eax,0
    in ax,dx
    ret

io_in32:
    mov edx,[esp+4]
    in eax,dx
    ret

; void io_out(8|16|32)(int port, int data);
io_out8:
    mov edx,[esp+4]
    mov al,[esp+8]
    out dx,al
    ret

io_out16:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,ax
    ret

io_out32:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,eax
    ret

; int io_load_eflags();
io_load_eflags:
    pushfd
    pop eax
    ret

; void io_store_eflags(int eflags);
io_store_eflags:
    mov eax,[esp+4]
    push eax
    popfd
    ret

; void load_gdtr(int limit, int addr);
load_gdtr:
    mov ax,[esp+4]
    mov [esp+6],ax
    lgdt [esp+6]
    ret

; void load_idtr(int limit, int addr);
load_idtr:
    mov ax,[esp+4]
    mov [esp+6],ax
    lidt [esp+6]
    ret

asm_interrupt_handler_21:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call interrupt_handler_21
    pop eax
    popad
    pop ds
    pp es
    iretd

asm_interrupt_handler_27:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call interrupt_handler_27
    pop eax
    popad
    pop ds
    pop es
    iretd

asm_interrupt_handler_2c:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call interrupt_handler_2c
    pop eax
    popad
    pop ds
    pop es
    iretd
