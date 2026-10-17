#include <stdarg.h>

extern int func_0013430c(char *destination, unsigned capacity,
                         const char *format, va_list arguments);

int func_0018fdb8(char *destination, unsigned capacity, const char *format, ...)
{
    int count;
    va_list arguments;
    va_start(arguments, format);
    count = func_0013430c(destination, capacity, format, arguments);
    va_end(arguments);
    return count;
}
