extern "C" {
void io_hlt();
void io_cli();
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);
}
void set_palette(int start, int end, unsigned char* rgb);
void init_palette();
void draw_box(unsigned char* vram, int vram_x_len, unsigned char color, int x0, int y0, int x1, int y1);
void os_putchar(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* font);
void init_screen(unsigned char* vram, int vram_x_len, int vram_y_len);

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

extern "C" void os_main()
{
    init_palette();

    struct BootInfo* boot_info = (struct BootInfo*)0x0ff0;

    init_screen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    static unsigned char font_A[] = {
        0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
    };
    os_putchar(boot_info->vram, boot_info->vram_x_len, 10, 10, kColorFFFFFF, font_A);
    while (1) {
        io_hlt();
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
