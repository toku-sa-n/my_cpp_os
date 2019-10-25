[bits 32]
    global IoHlt
    global IoCli
    global IoSti
    global IoStiHlt

    global IoIn8
    global IoIn16
    global IoIn32

    global IoOut8
    global IoOut16
    global IoOut32

    global IoLoadEflags
    global IoStoreEflags

    global LoadGdtr
    global LoadIdtr

    global AsmInterruptHandler21
    global AsmInterruptHandler27
    global AsmInterruptHandler2c

    extern InterruptHandler21
    extern InterruptHandler27
    extern InterruptHandler2c

[section .text]

IoHlt:
    hlt
    ret

IoCli:
    cli
    ret

IoSti:
    sti
    ret

IoStiHlt:
    sti
    hlt
    ret

; int IoIn(8|16|32)(int port);
IoIn8:
    mov edx,[esp+4]
    mov eax,0
    in al,dx
    ret

IoIn16:
    mov edx,[esp+4]
    mov eax,0
    in ax,dx
    ret

IoIn32:
    mov edx,[esp+4]
    in eax,dx
    ret

; void IoOut(8|16|32)(int port, int data);
IoOut8:
    mov edx,[esp+4]
    mov al,[esp+8]
    out dx,al
    ret

IoOut16:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,ax
    ret

IoOut32:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,eax
    ret

; int IoLoadEflags();
IoLoadEflags:
    pushfd
    pop eax
    ret

; void IoStoreEflags(int eflags);
IoStoreEflags:
    mov eax,[esp+4]
    push eax
    popfd
    ret

; void LoadGdtr(int limit, int addr);
LoadGdtr:
    mov ax,[esp+4]
    mov [esp+6],ax
    lgdt [esp+6]
    ret

; void LoadIdtr(int limit, int addr);
LoadIdtr:
    mov ax,[esp+4]
    mov [esp+6],ax
    lidt [esp+6]
    ret

AsmInterruptHandler21:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call InterruptHandler21
    pop eax
    popad
    pop ds
    pop es
    iretd

AsmInterruptHandler27:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call InterruptHandler27
    pop eax
    popad
    pop ds
    pop es
    iretd

AsmInterruptHandler2c:
    push es
    push ds
    pushad
    mov eax,esp
    push eax
    mov ax,ss
    mov ds,ax
    mov es,ax
    call InterruptHandler2c
    pop eax
    popad
    pop ds
    pop es
    iretd
