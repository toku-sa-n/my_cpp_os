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
// clang-format on

const int kPortKeyData = 0x0060;

void InitPic();

extern "C" {
void InterruptHandler21(int* esp);
void InterruptHandler27(int* esp);
void InterruptHandler2c(int* esp);
}
