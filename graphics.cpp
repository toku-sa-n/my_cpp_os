#include "graphics.h"
#include "asm_func.h"
#include "os.h"

void DrawBlock(unsigned char* vram, int vram_x_len, int pic_size_x, int pic_size_y, int pic_pos_x, int pic_pos_y, char* buf, int buf_size_x)
{
    for (int y = 0; y < pic_size_y; y++) {
        for (int x = 0; x < pic_size_x; x++) {
            vram[(pic_pos_y + y) * vram_x_len + (pic_pos_x + x)] = buf[y * buf_size_x + x];
        }
    }
}

void InitScreen(unsigned char* vram, int vram_x_len, int vram_y_len)
{
    // clang-format off
    DrawBox(vram, vram_x_len, kColorBackGround,               0,               0, vram_x_len -  1, vram_y_len - 29);
    DrawBox(vram, vram_x_len, kColorC6C6C6    ,               0, vram_y_len - 28, vram_x_len -  1, vram_y_len - 28);
    DrawBox(vram, vram_x_len, kColorFFFFFF    ,               0, vram_y_len - 27, vram_x_len -  1, vram_y_len - 27);
    DrawBox(vram, vram_x_len, kColorC6C6C6    ,               0, vram_y_len - 26, vram_x_len -  1, vram_y_len -  1);

    DrawBox(vram, vram_x_len, kColorFFFFFF    ,               3, vram_y_len - 24,              59, vram_y_len - 24);
    DrawBox(vram, vram_x_len, kColorFFFFFF    ,               2, vram_y_len - 24,               2, vram_y_len -  4);
    DrawBox(vram, vram_x_len, kColor848484    ,               3, vram_y_len -  4,              59, vram_y_len -  4);
    DrawBox(vram, vram_x_len, kColor848484    ,              59, vram_y_len - 23,              59, vram_y_len -  5);
    DrawBox(vram, vram_x_len, kColor000000    ,               2, vram_y_len -  3,              59, vram_y_len -  3);
    DrawBox(vram, vram_x_len, kColor000000    ,              60, vram_y_len - 24,              60, vram_y_len -  3);

    DrawBox(vram, vram_x_len, kColor848484    , vram_x_len - 47, vram_y_len - 24, vram_x_len -  4, vram_y_len - 24);
    DrawBox(vram, vram_x_len, kColor848484    , vram_x_len - 47, vram_y_len - 23, vram_x_len - 47, vram_y_len -  4);
    DrawBox(vram, vram_x_len, kColorFFFFFF    , vram_x_len - 47, vram_y_len -  3, vram_x_len -  4, vram_y_len -  3);
    DrawBox(vram, vram_x_len, kColorFFFFFF    , vram_x_len -  3, vram_y_len - 24, vram_x_len -  3, vram_y_len -  3);
    // clang-format on
}

void SetPalette(int start, int end, unsigned char* rgb)
{
    // See: http://oswiki.osask.jp/?VGA
    // ビデオDAコンバータの項
    int eflags = IoLoadEflags();
    IoCli();
    IoOut8(0x03c8, start);
    for (int i = start; i <= end; i++) {
        // You can specify the number between 0 and 63
        // as R, G and B so divide by 4 because the range of each elements of the array rgb is from 0 to 255.
        IoOut8(0x03c9, rgb[0] / 4);
        IoOut8(0x03c9, rgb[1] / 4);
        IoOut8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    IoStoreEflags(eflags);
}

void InitPalette()
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

    SetPalette(0, 15, rgb_table);
}

void DrawBox(unsigned char* vram, int vram_x_len, unsigned char color, int x0, int y0, int x1, int y1)
{
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            vram[y * vram_x_len + x] = color;
        }
    }
}

void OSPutChar(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* font)
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
void OSPuts(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* s)
{
    for (; *s != 0x00; s++) {
        OSPutChar(vram, vram_x_len, x, y, color, fonts + *s * 16);
        x += 8;
    }
}
