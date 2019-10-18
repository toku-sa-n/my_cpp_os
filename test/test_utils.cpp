#include <cstring>
#include <iostream>
#include <stdarg.h>

const int kBufferSize = 1024;

int os_vsprintf(char* str, const char* format, va_list ap);

bool test_os_vsprintf(const char* format, ...)
{
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

    return true;
}

int main()
{
    test_os_vsprintf("This");
    test_os_vsprintf("%d", 2);
    test_os_vsprintf("%d", 35);
}
