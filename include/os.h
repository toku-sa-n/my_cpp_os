// clang-format off
const int kAddrBootInfo = 0x00000ff0;

const int kAddrIdt      = 0x0026f800;
const int kLimitIdt     = 0x000007ff;
const int kAddrGdt      = 0x00270000;
const int kLimitGdt     = 0x0000ffff;
const int kAddrOSMain   = 0x00280000;
const int kLimitOSMain  = 0x0007ffff;
const int kAddrDataRw   = 0x4092;
const int kAddrCodeEr   = 0x409a;
const int kAddrIntGate  = 0x008e;
// clang-format on

struct BootInfo {
    char cyls, leds, vmode, reserve;
    short vram_x_len, vram_y_len;
    unsigned char* vram;
};
