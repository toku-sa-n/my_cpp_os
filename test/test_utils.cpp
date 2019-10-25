#include <cstring>
#include <iostream>
#include <stdarg.h>

const int kBufferSize = 1024;

int OSVSPrintf(char* str, const char* format, va_list ap);

bool TestOSVSPrintf(int& num_test, int& num_success, const char* format, ...)
{
    num_test++;
    va_list ap;

    va_start(ap, format);
    char os_buf[kBufferSize] = { '\0' };
    int os_count = OSVSPrintf(os_buf, format, ap);
    va_end(ap);

    va_start(ap, format);
    char builtin_buf[kBufferSize] = { '\0' };
    int builtin_count = vsprintf(builtin_buf, format, ap);
    va_end(ap);

    auto error_msg = [=](std::string message, auto expected, auto actual) {
        std::cout << std::endl;
        std::cout << "--------------------------------" << std::endl;
        std::cout << message << std::endl;
        std::cout << "  Format: " << format << std::endl;
        std::cout << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        std::cout << "  Actual: " << actual << std::endl;
        std::cout << "--------------------------------" << std::endl;
    };

    if (os_count != builtin_count) {
        error_msg("Wrong return value!", builtin_count, os_count);
        return false;
    }

    if (strcmp(os_buf, builtin_buf)) {
        error_msg("Test failed!", builtin_buf, os_buf);
        return false;
    }

    num_success++;
    return true;
}

int main()
{
    int num_test = 0, num_success = 0;
#define TEST_OS_VSPRINTF(format, ...) \
    TestOSVSPrintf(num_test, num_success, format, ##__VA_ARGS__)
    TEST_OS_VSPRINTF("This");

    TEST_OS_VSPRINTF("%d", 2);
    TEST_OS_VSPRINTF("%d", 35);
    TEST_OS_VSPRINTF("%d", 123456789);
    TEST_OS_VSPRINTF("%d", -3);
    TEST_OS_VSPRINTF("%2d", 3);
    TEST_OS_VSPRINTF("%5d", 3);
    TEST_OS_VSPRINTF("%3d%5d", 3, 5);
    TEST_OS_VSPRINTF("%03d%05d", 3, 5);

    TEST_OS_VSPRINTF("%X", 1);
    TEST_OS_VSPRINTF("%X", 10);
    TEST_OS_VSPRINTF("%X", 0xABC);
    TEST_OS_VSPRINTF("This is %X pens.", 0xABC);
    TEST_OS_VSPRINTF("%3X%5X", 3, 5);

    TEST_OS_VSPRINTF("%d%X", 334, 0x345);
#undef TEST_OS_VSPRINTF

    std::cout << "The number of tests  : " << num_test << std::endl;
    std::cout << "The number of success: " << num_success << std::endl;
}
