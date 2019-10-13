extern "C" {
void io_hlt();
}

void os_main()
{
    for (int i = 0xa0000; i <= 0xaffff; i++) {
        char* p = (char*)i;
        *p = i & 0x0f;
    }

    while (1) {
        io_hlt();
    }
}
