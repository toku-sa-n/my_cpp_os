#include <cstring>
#include <iostream>
#include <stdarg.h>

const int kBufferSize = 1024;

int os_vsprintf(char* str, const char* format, va_list ap);

bool test_os_vsprintf(int& num_test, int& num_success, const char* format, ...)
{
    num_test++;
    va_list ap;

    va_start(ap, format);
    char os_buf[kBufferSize] = { '\0' };
    int os_count = os_vsprintf(os_buf, format, ap);
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
    test_os_vsprintf(num_test, num_success, format, ##__VA_ARGS__)
    TEST_OS_VSPRINTF("This");
    TEST_OS_VSPRINTF("%d", 2);
    TEST_OS_VSPRINTF("%d", 35);
    TEST_OS_VSPRINTF("%d", 123456789);
#undef TEST_OS_VSPRINTF

    std::cout << "The number of tests  : " << num_test << std::endl;
    std::cout << "The number of success: " << num_success << std::endl;
}
