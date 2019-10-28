#include "interrupt.h"
#include "asm_func.h"
#include "graphics.h"
#include "os.h"
#include "utils.h"

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

void WaitKBCSendReady()
{
    while (1) {
        if (!(IoIn8(kPortKeyStatus) & kKeyStatusSendNotReady)) {
            break;
        }
    }
}

void InitKeyboard()
{
    WaitKBCSendReady();
    IoOut8(kPortKeyCmd, kKeyCmdWriteMode);
    WaitKBCSendReady();
    IoOut8(kPortKeyData, kKBCMode);
}

void EnableMouse(struct MouseDecoder& mouse_decoder)
{
    WaitKBCSendReady();
    IoOut8(kPortKeyCmd, kKeyCmdSendToMouse);
    WaitKBCSendReady();
    IoOut8(kPortKeyData, kMouseCmdEnable);

    mouse_decoder.phase = 0;
}

// Return true if the buffer of mouse_decoder is full or somwthing goes wrong.
// Otherwise return false.
bool decode_mouse(struct MouseDecoder& mouse_decoder, unsigned char data)
{
    switch (mouse_decoder.phase) {
    case 0:
        if (data == 0xfa) {
            mouse_decoder.phase = 1;
        }
        return false;
    case 1:
        mouse_decoder.buf[0] = data;
        mouse_decoder.phase = 2;
        return false;
    case 2:
        mouse_decoder.buf[1] = data;
        mouse_decoder.phase = 3;
        return false;
    case 3:
        mouse_decoder.buf[2] = data;
        mouse_decoder.phase = 1;
        return true;
    }
    return true;
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
    extern Queue<128> mouse_queue;
    mouse_queue.Enqueue(IoIn8(kPortKeyData));
}

void InterruptHandler27(int* esp)
{
    IoOut8(kPic0Ocw2, 0x67);
}
