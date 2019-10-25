int os_sprintf(char* str, const char* format, ...);

struct BootInfo {
    char cyls, leds, vmode, reserve;
    short vram_x_len, vram_y_len;
    unsigned char* vram;
};
