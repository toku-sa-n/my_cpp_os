#include <cstring>
#include <stdarg.h>

int os_vsprintf(char* str, const char* format, va_list ap)
{
    int count = 0;

    int format_len = strlen(format);
    for (int i = 0; i < format_len; i++) {
        str[i] = format[i];
        count++;
    }

    str[format_len] = '\0';

    return count;
}

int os_sprintf(char* str, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    int count = os_vsprintf(str, format, ap);

    va_end(ap);

    return count;
}
