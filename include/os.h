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
