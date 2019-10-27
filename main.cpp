#include "asm_func.h"
#include "descriptor_table.h"
#include "graphics.h"
#include "interrupt.h"
#include "os.h"
#include "utils.h"

Queue<32> key_queue;
Queue<128> mouse_queue;

void MainLoop(int& mouse_phrase)
{
    static unsigned char mouse_buf[3];
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;
    IoCli();
    if (key_queue.GetNumElements() + mouse_queue.GetNumElements() == 0) {
        IoStiHlt();
        return;
    }
    if (key_queue.GetNumElements()) {
        int key_data = key_queue.Dequeue();
        IoSti();
        char s[40];
        OSSPrintf(s, "%02X", key_data);
        DrawBox(boot_info->vram, boot_info->vram_x_len, kColor008484, 0, 16, 15, 31);
        OSPuts(boot_info->vram, boot_info->vram_x_len, 0, 16, kColorFFFFFF, (unsigned char*)s);
        return;
    }
    if (mouse_queue.GetNumElements()) {
        int mouse_data = mouse_queue.Dequeue();
        IoSti();

        switch (mouse_phrase) {
        case 0:
            if (mouse_data == 0xfa) {
                mouse_phrase = 1;
            }
            break;
        case 1:
            mouse_buf[0] = mouse_data;
            mouse_phrase++;
            break;
        case 2:
            mouse_buf[1] = mouse_data;
            mouse_phrase++;
            break;
        case 3:
            mouse_buf[2] = mouse_data;
            mouse_phrase = 1;

            char s[40];
            OSSPrintf(s, "%02X %02X %02X", mouse_buf[0], mouse_buf[1], mouse_buf[2]);
            DrawBox(boot_info->vram, boot_info->vram_x_len, kColor008484, 32, 16, 32 + 8 * 8 - 1, 31);
            OSPuts(boot_info->vram, boot_info->vram_x_len, 32, 16, kColorFFFFFF, (unsigned char*)s);
        }
    }
}

void InitOS()
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

    InitKeyboard();
    EnableMouse();
}

extern "C" void OSMain()
{
    InitOS();
    int mouse_phrase = 0;
    while (1) {
        MainLoop(mouse_phrase);
    }
}
