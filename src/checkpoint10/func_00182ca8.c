#include <stdarg.h>

extern int func_00144988(unsigned short *destination, unsigned capacity,
                         const unsigned short *format, va_list arguments);

int func_00182ca8(unsigned short *destination, unsigned capacity,
                  const unsigned short *format, ...)
{
    int result;
    va_list arguments;
    va_start(arguments, format);
    result = func_00144988(destination, capacity, format, arguments);
    va_end(arguments);
    return result;
}
