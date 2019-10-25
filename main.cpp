#include "asm_func.h"
#include "descriptor_table.h"
#include "graphics.h"
#include "interrupt.h"
#include "os.h"

extern "C" void os_main()
{
    init_palette();

    init_pic();

    init_gdt_idt();

    struct BootInfo* boot_info = (struct BootInfo*)0x0ff0;

    init_screen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    os_puts(boot_info->vram, boot_info->vram_x_len, 8, 8, kColorFFFFFF, (unsigned char*)"ABC 123");
    os_puts(boot_info->vram, boot_info->vram_x_len, 31, 31, kColor000000, (unsigned char*)"My C++ OS");
    os_puts(boot_info->vram, boot_info->vram_x_len, 30, 30, kColorFFFFFF, (unsigned char*)"My C++ OS");

    char s[40];
    os_sprintf(s, "screen_x = %d", boot_info->vram_x_len);
    os_puts(boot_info->vram, boot_info->vram_x_len, 16, 64, kColorFFFFFF, (unsigned char*)s);

    char buf_cursor[256];
    init_mouse_cursor((unsigned char*)buf_cursor, kColor008484);
    int mouse_x = (boot_info->vram_x_len - 16) / 2;
    int mouse_y = (boot_info->vram_y_len - 28 - 16) / 2;
    draw_block(boot_info->vram, boot_info->vram_x_len, 16, 16, mouse_x, mouse_y, buf_cursor, 16);

    while (1) {
        io_hlt();
    }
}
