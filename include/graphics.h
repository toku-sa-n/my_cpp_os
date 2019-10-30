#pragma once

extern unsigned char fonts[4096];

void SetPalette(int start, int end, unsigned char* rgb);
void InitPalette();
void DrawBox(unsigned char* vram, int vram_x_len, unsigned char color, int x0, int y0, int x1, int y1);
void OSPutChar(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* font);
void OSPuts(unsigned char* vram, int vram_x_len, int x, int y, char color, unsigned char* s);
void InitScreen(unsigned char* vram, int vram_x_len, int vram_y_len);
void InitMouseCursor(unsigned char* mouse, char background_color);
void DrawBlock(unsigned char* vram, int vram_x_len, int pic_size_x, int pic_size_y, int pic_pos_x, int pic_pos_y, char* buf, int buf_size_x);

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
