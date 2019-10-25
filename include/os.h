void init_gdt_idt();

int os_sprintf(char* str, const char* format, ...);

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
