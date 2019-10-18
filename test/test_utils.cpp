#include <cstring>
#include <iostream>
#include <stdarg.h>

const int kBufferSize = 1024;

int os_vsprintf(char* str, const char* format, va_list ap);

bool test_os_vsprintf(const char* format, ...)
{
    va_list ap;

    va_start(ap, format);
    char os_buf[kBufferSize];
    int os_count = os_vsprintf(os_buf, format, ap);
    va_end(ap);

    va_start(ap, format);
    char builtin_buf[kBufferSize];
    int builtin_count = os_vsprintf(builtin_buf, format, ap);
    va_end(ap);

    auto error_msg = [=](std::string message) {
        std::cout << message << std::endl;
        std::cout << "Format: " << format << std::endl;
        std::cout << std::endl;
        std::cout << "Expected: " << builtin_count << std::endl;
        std::cout << "Actual: " << os_count << std::endl;
    };

    if (os_count != builtin_count) {
        error_msg("Wrong return value!");
        return false;
    }

    if (strcmp(os_buf, builtin_buf)) {
        error_msg("Test failed!");
        return false;
    }

    return true;
}

int main()
{
    test_os_vsprintf("%d", 2);
}
