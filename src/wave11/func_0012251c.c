#include <stdarg.h>
#include <limits.h>

extern int func_0013430c(char *destination, unsigned capacity,
                         const char *format, va_list arguments);

int func_0012251c(char *destination, const char *format, ...)
{
    int count;
    va_list arguments;
    va_start(arguments, format);
    count = func_0013430c(destination, INT_MAX, format, arguments);
    va_end(arguments);
    return count;
}
