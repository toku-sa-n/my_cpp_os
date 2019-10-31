#include <cstdarg>

int OSSPrintf(char* str, const char* format, ...);
int OSVSPrintf(char* str, const char* format, va_list ap);

template <typename T>
T Between(const T value, const T from, const T to)
{
    return 3;
}
