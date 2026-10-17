#include "game/exp_heap.h"
#include <stdarg.h>
#include "game/text.h"



struct ResourceEntry *func_0015db7c(int screen, unsigned char flag,
    float x, float y, float z, unsigned color, unsigned capacity,
    const unsigned short *format, ...)
{
    va_list arguments;
    unsigned short *buffer;
    struct ResourceEntry *entry;
    buffer = func_0019ffa8(capacity * 2);
    va_start(arguments, format);
    func_0010083c(buffer, capacity, format, arguments);
    if (!capacity)
        capacity = (unsigned short)func_0017adc4(buffer);
    entry = func_0015d798(0, screen, flag, x, y, z, color, capacity, buffer);
    func_001852f8(buffer);
    va_end(arguments);
    return entry;
}
