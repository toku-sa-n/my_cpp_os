#pragma once

// clang-format off
const int kPic0Icw1 = 0x0020;
const int kPic0Ocw2 = 0x0020;
const int kPic0Imr  = 0x0021;
const int kPic0Icw2 = 0x0021;
const int kPic0Icw3 = 0x0021;
const int kPic0Icw4 = 0x0021;
const int kPic1Icw1 = 0x00a0;
const int kPic1Ocw2 = 0x00a0;
const int kPic1Imr  = 0x00a1;
const int kPic1Icw2 = 0x00a1;
const int kPic1Icw3 = 0x00a1;
const int kPic1Icw4 = 0x00a1;

const int kPortKeyData           = 0x0060;
const int kPortKeyStatus         = 0x0064;
const int kPortKeyCmd            = 0x0064;
const int kKeyStatusSendNotReady = 0x02;
const int kKeyCmdWriteMode       = 0x60;
const int kKBCMode               = 0x47;

const int kKeyCmdSendToMouse     = 0xd4;
const int kMouseCmdEnable        = 0xf4;
// clang-format on

class Mouse {
    unsigned char buf[3];
    unsigned char phase;

public:
    bool Decode(unsigned char data);
    void PutInfo(int x, int y);
    void Enable();
};

void InitKeyboard();
void InitPic();

bool decode_mouse(struct Mouse& mouse, unsigned char data);

extern "C" {
void InterruptHandler21(int* esp);
void InterruptHandler27(int* esp);
void InterruptHandler2c(int* esp);
}
