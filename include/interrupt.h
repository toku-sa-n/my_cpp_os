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

// This class handles about mouse cursor itself
// and doesn't contain queue for keeping data from mouse interruption.
// But for keeping the most recent three byte, buf_[3] can be used.
// This array is used to know where mouse moves to and which buttons are pressed.

/// @brief Class about mouse pointer.
/// @todo Extract fields and methods which are related to mouse device, not mouse pointer.
class MousePointer {
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

    /// @brief Next position of mouse pointer.
    int next_x_, next_y_;
    int moving_distance_x_, moving_distance_y_;
    int button_;
    unsigned char buf_color_[y_len_][x_len_];

    void SetColor(unsigned char background_color);

public:
    bool Decode(unsigned char data);

    /// @brief Put the information of mouse pointer on the screen
    /// @param x X coordinate of top left of output.
    /// @param y Y coordinate of top left of output.
    /// @details The format of information is as below.
    /// @n
    /// [lcr (X speed) (Y speed)]
    void PutInfo(int x, int y);

    /// @brief Put the coordinates of mouse pointer.
    /// @param x X coordinate of top left of output.
    /// @param y Y coordinate of top left of output.
    void PutPosition(int x, int y);

    /// @brief Draw mouse pointer on the screen.
    void Draw();

    /// @brief Enable mouse device.
    /// @todo Move this method to MouseDevice class.
    void Enable();

    /// @brief Set the next position where mouse pointer moves to.
    /// @todo Awkward! Improve this.
    void SetCoord(int x, int y);
};

void WaitKBCSendReady();
void InitKeyboard();
void InitPic();

extern "C" {
void InterruptHandler21(int* esp);
void InterruptHandler27(int* esp);
void InterruptHandler2c(int* esp);
}
