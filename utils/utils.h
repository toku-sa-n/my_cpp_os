#include <cstdarg>

int OSSPrintf(char* str, const char* format, ...);
int OSVSPrintf(char* str, const char* format, va_list ap);

template <typename T>
T Between(const T value, const T from, const T to)
{
    if (value < from) {
        return from;
    }

    if (value > to) {
        return to;
    }

    return value;
}
