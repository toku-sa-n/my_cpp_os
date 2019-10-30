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

#include "utils.h"

class Mouse {
    static const int x_len_ = 16, y_len_ = 16;
    inline static constexpr unsigned char buf_graphic_[y_len_ + 1][x_len_ + 1] {
        "*...............",
        "**..............",
        "*0*.............",
        "*00*............",
        "*000*...........",
        "*0000*..........",
        "*00000*.........",
        "*000000*........",
        "*0000000*.......",
        "*00000****......",
        "*00*00*.........",
        "*0*.*00*........",
        "**...*0*........",
        "*....*00*.......",
        "......***.......",
        "................",
    };

    unsigned char buf_[3];
    unsigned char phase_;
    int x_ = 100, y_ = 100;
    // SetCoord() assigns next_[xy]_.
    // When Draw() is called, mouse cursor will be drawn at (next_x_, next_y_).
    // Then [xy]_ and next_[xy]_ are swapped.
    // This process is needed to erase the previous mouse and draw the new one.
    int next_x_, next_y_;
    int moving_distance_x_, moving_distance_y_;
    int button_;
    unsigned char buf_color_[y_len_][x_len_];
    Queue<128> queue_;

    void SetColor(unsigned char background_color);

public:
    bool Decode(unsigned char data);
    bool IsQueueEmpty();
    void Enqueue(unsigned char data);
    unsigned char Dequeue();

    void PutInfo(int x, int y);
    void PutPosition(int x, int y);
    void Draw();

    void Enable();
    void SetCoord(int x, int y);
};

void WaitKBCSendReady();
void InitKeyboard();
void InitPic();

bool decode_mouse(struct Mouse& mouse, unsigned char data);

extern "C" {
void InterruptHandler21(int* esp);
void InterruptHandler27(int* esp);
void InterruptHandler2c(int* esp);
}
