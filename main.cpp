#include "asm_func.h"
#include "descriptor_table.h"
#include "graphics.h"
#include "interrupt.h"
#include "memory.h"
#include "os.h"
#include "utils.h"

Queue<32> key_queue;
Queue<128> mouse_queue;

void MainLoop(MouseDevice& mouse_device, MousePointer& mouse_pointer)
{
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
        DrawBox(boot_info->vram, boot_info->vram_x_len, kColorBackGround, 0, 16, 15, 31);
        OSPuts(boot_info->vram, boot_info->vram_x_len, 0, 16, kColorFFFFFF, (unsigned char*)s);
        return;
    }
    if (mouse_queue.GetNumElements()) {
        int mouse_data = mouse_queue.Dequeue();
        IoSti();

        if (mouse_device.PutInterruptionData(mouse_data, mouse_pointer)) {
            mouse_device.PutInfo(16, 32);
            mouse_pointer.PutPosition(0, 0);
            mouse_pointer.Draw();
        }
    }
}

void InitOS(MouseDevice& mouse_device, MousePointer& mouse_pointer)
{
    InitGdtIdt();
    InitPic();

    IoSti();

    InitPalette();
    InitScreen(boot_info->vram, boot_info->vram_x_len, boot_info->vram_y_len);

    mouse_pointer.PutPosition(0, 0);

    IoOut8(kPic0Imr, 0xf9); // Accept interrupt from Pic1 and keyboard
    IoOut8(kPic1Imr, 0xef); // Accept interrupt from mouse

    InitKeyboard();
    mouse_device.Enable();

    char s[40];
    OSSPrintf(s, "memory %dMB", CheckMemorySize(0x00400000, 0xbfffffff) / (1024 * 1024));
    OSPuts(boot_info->vram, boot_info->vram_x_len, 0, 32, kColorFFFFFF, (unsigned char*)s);

    mouse_pointer.Draw();
}

extern "C" void OSMain()
{
    MousePointer mouse_pointer;
    MouseDevice mouse_device;
    InitOS(mouse_device, mouse_pointer);

    while (1) {
        MainLoop(mouse_device, mouse_pointer);
    }
}
