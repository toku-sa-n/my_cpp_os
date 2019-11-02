#pragma once
#include "utils.h"

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

    int x_ = 100, y_ = 100;
    // SetCoord() assigns next_[xy]_.
    // When Draw() is called, mouse cursor will be drawn at (next_x_, next_y_).
    // Then [xy]_ and next_[xy]_ are swapped.
    // This process is needed to erase the previous mouse and draw the new one.

    /// @brief Next position of mouse pointer.
    int next_x_, next_y_;
    unsigned char buf_color_[y_len_][x_len_];

public:
    /// @brief Assign color indexes corresponding to each elements of buf_graphic_ and move mouse cursor position to the center of the screen.
    MousePointer();

    /// @brief Put the coordinates of mouse pointer.
    /// @param x X coordinate of top left of output.
    /// @param y Y coordinate of top left of output.
    void PutPosition(int x, int y);

    /// @brief Draw mouse pointer on the screen.
    void Draw();

    /// @brief Move mouse cursor relatively.
    void MoveBy(int x, int y);
};

/// @brief Class about mouse device.
class MouseDevice {
    unsigned char buf_[3];
    unsigned char phase_;

    int moving_distance_x_, moving_distance_y_;
    int button_;

    void DecodeDataInBuf(MousePointer& mouse_pointer);

public:
    /// @brief Put a data sent by mouse device interruption into 3-bytes buffer. If buffer becomes full, get informations about the movement of mouse device.
    /// @param data Data to put into a buffer.
    /// @param mouse_pointer A mouse_pointer handler. When buffer becomes full, information will be sent to it.
    /// @return Return true if buffer becomes full or something wrong, otherwise return false.
    bool PutInterruptionData(unsigned char data, MousePointer& mouse_pointer);

    /// @brief Put the information of mouse pointer on the screen
    /// @param x X coordinate of top left of output.
    /// @param y Y coordinate of top left of output.
    /// @details The format of information is as below.
    /// @n
    /// [lcr (X speed) (Y speed)]
    /// If left, center or right button is pressed, l, c or r will be upper respectively.
    void PutInfo(int x, int y);

    /// @brief Enable mouse device.
    /// @todo Move this method to MouseDevice class.
    void Enable();
};

void WaitKBCSendReady();
void InitKeyboard();
void InitPic();

extern "C" {
void InterruptHandler21(int* esp);
void InterruptHandler27(int* esp);
void InterruptHandler2c(int* esp);
}
