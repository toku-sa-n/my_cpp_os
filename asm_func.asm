[bits 32]
    global io_hlt
    global write_to_memory

[section .text]

io_hlt:
    hlt
    ret

write_to_memory:            ; void write_to_memory(int address, int data);
    mov ecx,[esp+4]
    mov al,[esp+8]
    mov [ecx],al
    ret
