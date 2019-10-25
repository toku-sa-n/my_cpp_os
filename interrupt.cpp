#include "interrupt.h"
#include "asm_func.h"
void init_pic()
{
    io_out8(Pic0Imr, 0xff); // Master ignores all interrupt signals.
    io_out8(Pic1Imr, 0xff); // Slave  ignores all interrupt signals.

    io_out8(Pic0Icw1, 0x11);   // Edge trigger mode.
    io_out8(Pic0Icw2, 0x20);   // Int20-27 accepted Irq0-7 respectively.
    io_out8(Pic0Icw3, 1 << 2); // Pic1 is connected with Irq2.
    io_out8(Pic0Icw4, 0x01);   // Non-buffer mode

    io_out8(Pic1Icw1, 0x11); // Edge trigger mode.
    io_out8(Pic1Icw2, 0x28); // Int28-2f accepts Irq8-15 respectively.
    io_out8(Pic1Icw3, 2);    // Pic1 is connected with Irq2.
    io_out8(Pic1Icw4, 0x01); // Non-buffer mode

    io_out8(Pic0Imr, 0xfb); // Master ignores all interrupt signals except from Pic1.
    io_out8(Pic1Imr, 0xff); // Slave ignores all interrupt signals.
}
