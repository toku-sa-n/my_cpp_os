extern "C" {
void io_hlt();
void io_cli();
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);
void os_main();
}
void set_palette(int start, int end, unsigned char* rgb);
void init_palette();

// Don't move os_main because this function must come after the label `bootpack:' which is in head.asm
void os_main()
{
    init_palette();

    char* p = (char*)0xa0000;

    for (int i = 0; i <= 0xffff; i++) {
        p[i] = i & 0x0f;
    }

    while (1) {
        io_hlt();
    }
}

void set_palette(int start, int end, unsigned char* rgb)
{
    // See: http://oswiki.osask.jp/?VGA
    // ビデオDAコンバータの項
    int eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for (int i = start; i <= end; i++) {
        // You can specify the number between 0 and 63
        // as R, G and B so divide by 4 because the range of each elements of the array rgb is from 0 to 255.
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
}

void init_palette()
{
    static unsigned char rgb_table[16 * 3] = {
        // clang-format off
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0xff, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84,
        // clang-format on
    };

    set_palette(0, 15, rgb_table);
}
