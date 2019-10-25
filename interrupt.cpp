#include "interrupt.h"
#include "asm_func.h"
#include "graphics.h"
#include "os.h"
#include "utils.h"

void init_pic()
{
    io_out8(kPic0Imr, 0xff); // Master ignores all interrupt signals.
    io_out8(kPic1Imr, 0xff); // Slave  ignores all interrupt signals.

    io_out8(kPic0Icw1, 0x11);   // Edge trigger mode.
    io_out8(kPic0Icw2, 0x20);   // Int20-27 accepted Irq0-7 respectively.
    io_out8(kPic0Icw3, 1 << 2); // Pic1 is connected with Irq2.
    io_out8(kPic0Icw4, 0x01);   // Non-buffer mode

    io_out8(kPic1Icw1, 0x11); // Edge trigger mode.
    io_out8(kPic1Icw2, 0x28); // Int28-2f accepts Irq8-15 respectively.
    io_out8(kPic1Icw3, 2);    // Pic1 is connected with Irq2.
    io_out8(kPic1Icw4, 0x01); // Non-buffer mode

    io_out8(kPic0Imr, 0xfb); // Master ignores all interrupt signals except from Pic1.
    io_out8(kPic1Imr, 0xff); // Slave ignores all interrupt signals.
}

void interrupt_handler_21(int* esp)
{
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;
    io_out8(kPic0Ocw2, 0x61);

    unsigned char data = io_in8(kPortKeyData);

    unsigned char s[4];
    os_sprintf((char*)s, "%02X", data);
    draw_box(boot_info->vram, boot_info->vram_x_len, kColor008484, 0, 16, 15, 31);
    os_puts(boot_info->vram, boot_info->vram_x_len, 0, 16, kColorFFFFFF, s);
}

void interrupt_handler_2c(int* esp)
{
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;
    draw_box(boot_info->vram, boot_info->vram_x_len, kColor000000, 0, 0, 32 * 8 - 1, 15);
    os_puts(boot_info->vram, boot_info->vram_x_len, 0, 0, kColorFFFFFF, (unsigned char*)"INT 2C (IRQ-12) : PS/2 mouse");

    while (1) {
        io_hlt();
    }
}

void interrupt_handler_27(int* esp)
{
    io_out8(kPic0Ocw2, 0x67);
}
