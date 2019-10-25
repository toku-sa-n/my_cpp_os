#include <cstring>
#include <stdarg.h>

static int int_to_chars(char** str, int n, int base, bool zero_flag, int digits_num)
{
    char numbers[] = "0123456789ABCDEF";
    char buf[1024] = { '\0' };

    int ptr = 0;
    int digits = 0;

    bool minus_flag = false;
    if (n < 0) {
        n = -n;
        minus_flag = true;
    }

    while (n > 0) {
        buf[ptr++] = numbers[n % base];
        n /= base;
        digits++;
    }

    if (minus_flag) {
        buf[ptr++] = '-';
        digits++;
    }

    int num_padding = digits_num - digits;
    for (int i = 0; i < num_padding; i++) {
        buf[ptr++] = (zero_flag ? '0' : ' ');
        digits++;
    }

    for (int i = 0; i < ptr / 2; i++) {
        char temp = buf[i];
        buf[i] = buf[ptr - 1 - i];
        buf[ptr - 1 - i] = temp;
    }

    for (int i = 0; i < ptr; i++) {
        *(*str)++ = buf[i];
    }

    return digits;
}

int os_vsprintf(char* str, const char* format, va_list ap)
{
    int count = 0;

    while (*format != '\0') {
        if (*format != '%') {
            *str++ = *format++;
            count++;
            continue;
        }

        format++; // format points '%' so move it by 1.

        bool zero_flag = false;
        if (*format == '0') {
            zero_flag = true;
        }

        int digits_num = 0;
        while (*format >= '0' && *format <= '9') {
            digits_num *= 10;
            digits_num += *format++ - '0';
        }

        if (*format == 'd') {
            count += int_to_chars(&str, va_arg(ap, int), 10, zero_flag, digits_num);
        } else if (*format == 'X') {
            count += int_to_chars(&str, va_arg(ap, int), 16, zero_flag, digits_num);
        }
        format++;
    }

    *str = '\0';
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
