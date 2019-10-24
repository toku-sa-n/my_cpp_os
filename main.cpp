extern "C" {
void io_hlt();
void io_cli();
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
}
void set_palette(int start, int end, unsigned char* rgb);
void init_palette();
void draw_box(unsigned char* vram, int vram_x_len, unsigned char color, int x0, int y0, int x1, int y1);
void os_putchar(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* font);
void os_puts(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* s);
void init_screen(unsigned char* vram, int vram_x_len, int vram_y_len);
void init_mouse_cursor(unsigned char* mouse, char background_color);
void draw_block(unsigned char* vram, int vram_x_len, int pic_size_x, int pic_size_y, int pic_pos_x, int pic_pos_y, char* buf, int buf_size_x);
void init_gdt_idt();

int os_sprintf(char* str, const char* format, ...);

const int kColor000000 = 0;
const int kColorFF0000 = 1;
const int kColor00FF00 = 2;
const int kColorFFFF00 = 3;
const int kColor0000FF = 4;
const int kColorFF00FF = 5;
const int kColor00FFFF = 6;
const int kColorFFFFFF = 7;
const int kColorC6C6C6 = 8;
const int kColor840000 = 9;
const int kColor008400 = 10;
const int kColor848400 = 11;
const int kColor000084 = 12;
const int kColor840084 = 13;
const int kColor008484 = 14;
const int kColor848484 = 15;

struct BootInfo {
    char cyls, leds, vmode, reserve;
    short vram_x_len, vram_y_len;
    unsigned char* vram;
};

struct SegmentDescriptor {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GateDescriptor {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

extern unsigned char fonts[4096];

extern "C" void os_main()
{
    init_palette();

    init_gdt_idt();

    struct BootInfo* boot_info = (struct BootInfo*)0x0ff0;

    init_screen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    os_puts(boot_info->vram, boot_info->vram_x_len, 8, 8, kColorFFFFFF, (unsigned char*)"ABC 123");
    os_puts(boot_info->vram, boot_info->vram_x_len, 31, 31, kColor000000, (unsigned char*)"My C++ OS");
    os_puts(boot_info->vram, boot_info->vram_x_len, 30, 30, kColorFFFFFF, (unsigned char*)"My C++ OS");

    char s[40];
    os_sprintf(s, "screen_x = %d", boot_info->vram_x_len);
    os_puts(boot_info->vram, boot_info->vram_x_len, 16, 64, kColorFFFFFF, (unsigned char*)s);

    char buf_cursor[256];
    init_mouse_cursor((unsigned char*)buf_cursor, kColor008484);
    int mouse_x = (boot_info->vram_x_len - 16) / 2;
    int mouse_y = (boot_info->vram_y_len - 28 - 16) / 2;
    draw_block(boot_info->vram, boot_info->vram_x_len, 16, 16, mouse_x, mouse_y, buf_cursor, 16);

    while (1) {
        io_hlt();
    }
}

void set_segment_descriptor(struct SegmentDescriptor* sd, unsigned int limit, int base, int access_right)
{
    if (limit > 0xfffff) {
        access_right |= 0x8000;
        limit /= 0x1000;
    }

    // clang-format off
    sd->limit_low    = limit & 0xffff;
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->access_right = access_right & 0xff;
    sd->limit_high   = ((limit >> 16) & 0x0f) | ((access_right >> 8) & 0xf0);
    sd->base_high    = (base >> 24) & 0xff;
    // clang-format on
}

void set_gate_descriptor(struct GateDescriptor* gd, int offset, int selector, int access_right)
{
    // clang-format off
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (access_right >> 8) & 0xff;
    gd->access_right = access_right & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;
    // clang-format on
}

void init_gdt_idt()
{
    // 0x270000 ~ 0x27ffff: GDT
    struct SegmentDescriptor* gdt = (struct SegmentDescriptor*)0x00270000;

    // Initialize all GDT segments
    // Limit: 0, Base: 0, Access right: 0
    for (int i = 0; i < 8192; i++) {
        set_segment_descriptor(gdt + i, 0, 0, 0);
    }

    set_segment_descriptor(gdt + 1, 0xffffffff, 0x00000000, 0x4092);

    // 0x002800: The start position of OS body file
    set_segment_descriptor(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);

    struct GateDescriptor* idt = (struct GateDescriptor*)0x0026f800;
    for (int i = 0; i < 256; i++) {
        set_gate_descriptor(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);
}

void draw_block(unsigned char* vram, int vram_x_len, int pic_size_x, int pic_size_y, int pic_pos_x, int pic_pos_y, char* buf, int buf_size_x)
{
    for (int y = 0; y < pic_size_y; y++) {
        for (int x = 0; x < pic_size_x; x++) {
            vram[(pic_pos_y + y) * vram_x_len + (pic_pos_x + x)] = buf[y * buf_size_x + x];
        }
    }
}

void init_mouse_cursor(unsigned char* mouse, char background_color)
{
    static unsigned char cursor[17][17] = {
        "*...............",
        "**..............",
        "*0*.............",
        "*00*............",
        "*000*...........",
        "*0000*..........",
        "*00000*.........",
        "*000000*........",
        "*0000000*.......",
        "*00000****......",
        "*00*00*.........",
        "*0*.*00*........",
        "**...*0*........",
        "*....*00*.......",
        "......***.......",
        "................",
    };

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            switch (cursor[y][x]) {
            case '*':
                mouse[y * 16 + x] = kColor000000;
                break;
            case '0':
                mouse[y * 16 + x] = kColorFFFFFF;
                break;
            case '.':
                mouse[y * 16 + x] = background_color;
                break;
            }
        }
    }
}

void init_screen(unsigned char* vram, int vram_x_len, int vram_y_len)
{
    // clang-format off
    draw_box(vram, vram_x_len, kColor008484,               0,               0, vram_x_len -  1, vram_y_len - 29);
    draw_box(vram, vram_x_len, kColorC6C6C6,               0, vram_y_len - 28, vram_x_len -  1, vram_y_len - 28);
    draw_box(vram, vram_x_len, kColorFFFFFF,               0, vram_y_len - 27, vram_x_len -  1, vram_y_len - 27);
    draw_box(vram, vram_x_len, kColorC6C6C6,               0, vram_y_len - 26, vram_x_len -  1, vram_y_len -  1);

    draw_box(vram, vram_x_len, kColorFFFFFF,               3, vram_y_len - 24,              59, vram_y_len - 24);
    draw_box(vram, vram_x_len, kColorFFFFFF,               2, vram_y_len - 24,               2, vram_y_len -  4);
    draw_box(vram, vram_x_len, kColor848484,               3, vram_y_len -  4,              59, vram_y_len -  4);
    draw_box(vram, vram_x_len, kColor848484,              59, vram_y_len - 23,              59, vram_y_len -  5);
    draw_box(vram, vram_x_len, kColor000000,               2, vram_y_len -  3,              59, vram_y_len -  3);
    draw_box(vram, vram_x_len, kColor000000,              60, vram_y_len - 24,              60, vram_y_len -  3);

    draw_box(vram, vram_x_len, kColor848484, vram_x_len - 47, vram_y_len - 24, vram_x_len -  4, vram_y_len - 24);
    draw_box(vram, vram_x_len, kColor848484, vram_x_len - 47, vram_y_len - 23, vram_x_len - 47, vram_y_len -  4);
    draw_box(vram, vram_x_len, kColorFFFFFF, vram_x_len - 47, vram_y_len -  3, vram_x_len -  4, vram_y_len -  3);
    draw_box(vram, vram_x_len, kColorFFFFFF, vram_x_len -  3, vram_y_len - 24, vram_x_len -  3, vram_y_len -  3);
    // clang-format on
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

void draw_box(unsigned char* vram, int vram_x_len, unsigned char color, int x0, int y0, int x1, int y1)
{
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            vram[y * vram_x_len + x] = color;
        }
    }
}

void os_putchar(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* font)
{
    for (int i = 0; i < 16; i++) {
        unsigned char* p = vram + (y + i) * vram_x_len + x;
        char d = font[i];
        for (int j = 0; j < 8; j++) {
            if (d & (1 << (7 - j))) {
                p[j] = color;
            }
        }
    }
}

// This function doesn't put '\n' at the end of sentence.
void os_puts(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* s)
{
    for (; *s != 0x00; s++) {
        os_putchar(vram, vram_x_len, x, y, color, fonts + *s * 16);
        x += 8;
    }
}
