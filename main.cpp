extern "C" {
void io_hlt();
void write_to_memory(int address, int data);
}

void os_main()
{
    for (int i = 0xa0000; i <= 0xaffff; i++) {
        write_to_memory(i, i & 0x0f);
    }

    while (1) {
        io_hlt();
    }
}
