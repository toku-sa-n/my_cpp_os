#include "asm_func.h"
#include "descriptor_table.h"
#include "graphics.h"
#include "interrupt.h"
#include "os.h"
#include "utils.h"

extern "C" void os_main()
{
    init_gdt_idt();
    init_pic();

    io_sti();

    init_palette();
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;

    init_screen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    char buf_cursor[256];
    init_mouse_cursor((unsigned char*)buf_cursor, kColor008484);
    int mouse_x = (boot_info->vram_x_len - 16) / 2;
    int mouse_y = (boot_info->vram_y_len - 28 - 16) / 2;
    draw_block(boot_info->vram, boot_info->vram_x_len, 16, 16, mouse_x, mouse_y, buf_cursor, 16);
    char s[40];
    os_sprintf(s, "(%d, %d)", mouse_x, mouse_y);
    os_puts(boot_info->vram, boot_info->vram_x_len, 0, 0, kColorFFFFFF, (unsigned char*)s);

    io_out8(kPic0Imr, 0xf9); // Accept interrupt from Pic1 and keyboard
    io_out8(kPic1Imr, 0xef); // Accept interrupt from mouse

    while (1) {
        io_hlt();
    }
}
