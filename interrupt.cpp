#include "interrupt.h"
#include "asm_func.h"
#include "graphics.h"
#include "os.h"
#include "utils.h"

void WaitKBCSendReady()
{
    while (1) {
        if (!(IoIn8(kPortKeyStatus) & kKeyStatusSendNotReady)) {
            break;
        }
    }
}

bool Mouse::Decode(unsigned char data)
{
    switch (phase_) {
    case 0:
        if (data == 0xfa) {
            phase_ = 1;
        }
        return false;
    case 1:
        buf_[0] = data;
        phase_ = 2;
        return false;
    case 2:
        buf_[1] = data;
        phase_ = 3;
        return false;
    case 3:
        buf_[2] = data;
        phase_ = 1;
        return true;
    }
    return true;
}

void Mouse::PutInfo(int x, int y)
{
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;

    char s[40];
    OSSPrintf(s, "%02X %02X %02X", buf_[0], buf_[1], buf_[2]);
    DrawBox(boot_info->vram, boot_info->vram_x_len, kColor008484, x, y, x + 8 * 8 - 1, y + 15);
    OSPuts(boot_info->vram, boot_info->vram_x_len, x, y, kColorFFFFFF, (unsigned char*)s);
}

void Mouse::SetColor(unsigned char background_color)
{
    for (int y = 0; y < y_len_; y++) {
        for (int x = 0; x < x_len_; x++) {
            switch (buf_graphic_[y][x]) {
            case '*':
                buf_color_[y][x] = (unsigned char)kColor000000;
                break;
            case '0':
                buf_color_[y][x] = (unsigned char)kColorFFFFFF;
                break;
            case '.':
                buf_color_[y][x] = (unsigned char)background_color;
                break;
            }
        }
    }
}

void Mouse::Enable()
{
    WaitKBCSendReady();
    IoOut8(kPortKeyCmd, kKeyCmdSendToMouse);
    WaitKBCSendReady();
    IoOut8(kPortKeyData, kMouseCmdEnable);

    phase_ = 0;
    SetColor(kColor008484);
    Draw();
}

bool Mouse::IsQueueEmpty()
{
    return queue_.GetNumElements() == 0;
}

unsigned char Mouse::Dequeue()
{
    return queue_.Dequeue();
}

void Mouse::Enqueue(unsigned char data)
{
    queue_.Enqueue(data);
}

void Mouse::SetCoord(int x, int y)
{
    x_ = x;
    y_ = y;
}

void Mouse::Draw()
{
    const struct BootInfo* boot_info = (const struct BootInfo*)kAddrBootInfo;
    DrawBlock(boot_info->vram, boot_info->vram_x_len, x_len_, y_len_, x_, y_, (char*)buf_color_, x_len_);
}

void Mouse::PutPosition(int x, int y)
{
    const struct BootInfo* boot_info = (const struct BootInfo*)kAddrBootInfo;
    char s[40];
    OSSPrintf(s, "(%d, %d)", x_, y_);
    OSPuts(boot_info->vram, boot_info->vram_x_len, x, y, kColorFFFFFF, (unsigned char*)s);
}

void InitPic()
{
    IoOut8(kPic0Imr, 0xff); // Master ignores all interrupt signals.
    IoOut8(kPic1Imr, 0xff); // Slave  ignores all interrupt signals.

    IoOut8(kPic0Icw1, 0x11);   // Edge trigger mode.
    IoOut8(kPic0Icw2, 0x20);   // Int20-27 accepted Irq0-7 respectively.
    IoOut8(kPic0Icw3, 1 << 2); // Pic1 is connected with Irq2.
    IoOut8(kPic0Icw4, 0x01);   // Non-buffer mode

    IoOut8(kPic1Icw1, 0x11); // Edge trigger mode.
    IoOut8(kPic1Icw2, 0x28); // Int28-2f accepts Irq8-15 respectively.
    IoOut8(kPic1Icw3, 2);    // Pic1 is connected with Irq2.
    IoOut8(kPic1Icw4, 0x01); // Non-buffer mode

    IoOut8(kPic0Imr, 0xfb); // Master ignores all interrupt signals except from Pic1.
    IoOut8(kPic1Imr, 0xff); // Slave ignores all interrupt signals.
}

void InitKeyboard()
{
    WaitKBCSendReady();
    IoOut8(kPortKeyCmd, kKeyCmdWriteMode);
    WaitKBCSendReady();
    IoOut8(kPortKeyData, kKBCMode);
}

void InterruptHandler21(int* esp)
{
    IoOut8(kPic0Ocw2, 0x61);
    extern Queue<32> key_queue;
    key_queue.Enqueue(IoIn8(kPortKeyData));
}

void InterruptHandler2c(int* esp)
{
    IoOut8(kPic1Ocw2, 0x64);
    IoOut8(kPic0Ocw2, 0x62);
    extern Mouse mouse;
    mouse.Enqueue(IoIn8(kPortKeyData));
}

void InterruptHandler27(int* esp)
{
    IoOut8(kPic0Ocw2, 0x67);
}
