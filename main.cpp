#include "asm_func.h"
#include "descriptor_table.h"
#include "graphics.h"
#include "interrupt.h"
#include "os.h"
#include "utils.h"

Queue<32> key_queue;

extern "C" void OSMain()
{
    InitGdtIdt();
    InitPic();

    IoSti();

    InitPalette();
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;

    InitScreen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    char buf_cursor[256];
    InitMouseCursor((unsigned char*)buf_cursor, kColor008484);
    int mouse_x = (boot_info->vram_x_len - 16) / 2;
    int mouse_y = (boot_info->vram_y_len - 28 - 16) / 2;
    DrawBlock(boot_info->vram, boot_info->vram_x_len, 16, 16, mouse_x, mouse_y, buf_cursor, 16);
    char s[40];
    OSSPrintf(s, "(%d, %d)", mouse_x, mouse_y);
    OSPuts(boot_info->vram, boot_info->vram_x_len, 0, 0, kColorFFFFFF, (unsigned char*)s);

    IoOut8(kPic0Imr, 0xf9); // Accept interrupt from Pic1 and keyboard
    IoOut8(kPic1Imr, 0xef); // Accept interrupt from mouse

    while (1) {
        IoCli();
        if (!key_queue.GetNumElements()) {
            IoStiHlt();
        } else {
            int key_data = key_queue.Dequeue();
            IoSti();
            OSSPrintf(s, "%02X", key_data);
            DrawBox(boot_info->vram, boot_info->vram_x_len, kColor008484, 0, 16, 15, 31);
            OSPuts(boot_info->vram, boot_info->vram_x_len, 0, 16, kColorFFFFFF, (unsigned char*)s);
        }
    }
}
