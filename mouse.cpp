#include "asm_func.h"
#include "graphics.h"
#include "interrupt.h"
#include "os.h"

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

bool Mouse::IsQueueEmpty()
{
    return queue_.GetNumElements() == 0;
}

void Mouse::Enqueue(unsigned char data)
{
    queue_.Enqueue(data);
}

unsigned char Mouse::Dequeue()
{
    return queue_.Dequeue();
}

void Mouse::PutInfo(int x, int y)
{
    struct BootInfo* boot_info = (struct BootInfo*)kAddrBootInfo;

    char s[40];
    OSSPrintf(s, "%02X %02X %02X", buf_[0], buf_[1], buf_[2]);
    DrawBox(boot_info->vram, boot_info->vram_x_len, kColor008484, x, y, x + 8 * 8 - 1, y + 15);
    OSPuts(boot_info->vram, boot_info->vram_x_len, x, y, kColorFFFFFF, (unsigned char*)s);
}

void Mouse::PutPosition(int x, int y)
{
    const struct BootInfo* boot_info = (const struct BootInfo*)kAddrBootInfo;
    char s[40];
    OSSPrintf(s, "(%d, %d)", x_, y_);
    OSPuts(boot_info->vram, boot_info->vram_x_len, x, y, kColorFFFFFF, (unsigned char*)s);
}

void Mouse::Draw()
{
    const struct BootInfo* boot_info = (const struct BootInfo*)kAddrBootInfo;
    DrawBlock(boot_info->vram, boot_info->vram_x_len, x_len_, y_len_, x_, y_, (char*)buf_color_, x_len_);
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

void Mouse::SetCoord(int x, int y)
{
    x_ = x;
    y_ = y;
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
