#include <cstring>
#include <stdarg.h>

int os_vsprintf(char* str, const char* format, va_list ap)
{
}

int os_sprintf(char* str, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    int count = os_vsprintf(str, format, ap);

    va_end(ap);

    return count;
}
