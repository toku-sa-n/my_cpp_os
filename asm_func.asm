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

    global LoadCr0
    global StoreCr0

    global AsmCheckMemorySize

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

%macro ASM_INTERRUPT_HANDLER 1
    AsmInterruptHandler%1:
        push es
        push ds
        pushad
        mov eax,esp
        push eax
        mov ax,ss
        mov ds,ax
        mov es,ax
        call InterruptHandler%1
        pop eax
        popad
        pop ds
        pop es
        iretd
%endmacro

ASM_INTERRUPT_HANDLER 21
ASM_INTERRUPT_HANDLER 27
ASM_INTERRUPT_HANDLER 2c

LoadCr0:
    mov eax,cr0
    ret

StoreCr0:
    mov eax,[esp+4]
    mov cr0,eax
    ret

; unsigned int AsmCheckMemorySize(unsigned start, unsigned end)
AsmCheckMemorySize:
    push edi
    push esi
    push ebx
    mov esi,0xaa55aa55
    mov edi,0x55aa55aa

    ; eax = start;
    mov eax,[esp+12+4]

; Assign a value into [eax] and reverse all bits of that place.
; The bits reversion will be done twice.
; If the reversion succeeds, then go to the next bit.
; Otherwise regard the last-succeess bit as the limit of memory.
; eax: Iterator
; ebx: Memory to check
; edx: Data on [ebx]. When checking memory, the value of checked memory will be changed.
; edx will be used to save the value in the memory.
check_memory_size_loop:
    mov ebx,eax
    add ebx,0xffc
    mov edx,[ebx]
    mov [ebx],esi
    xor dword [ebx],0xffffffff

    ; If bits reversion fails, end loop.
    cmp edi,[ebx]
    jne check_memory_size_fin

    ; Second trial of reversing bits.
    xor dword [ebx],0xffffffff
    cmp esi,[ebx]
    jne check_memory_size_fin

    ; Restore the value of [ebx]
    mov [ebx],edx

    ; Loop continuation condition.
    add eax,0x1000
    cmp eax,[esp+12+8]
    jbe check_memory_size_loop

check_memory_size_fin:
    mov [ebx],edx
    pop ebx
    pop esi
    pop edi
    ret
