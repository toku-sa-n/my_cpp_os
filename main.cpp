extern "C" {
void io_hlt();
}

void os_main()
{
fin:
    io_hlt();
    goto fin;
}
