#include "game/functions.h"

static unsigned char byte_at(unsigned value, unsigned index)
{
    return value >> (index * 8);
}

unsigned int func_0016b404(const unsigned int *source)
{
    unsigned word = *source;
    return (((unsigned)byte_at(word, 0)) << 24) | (((unsigned)byte_at(word, 1)) << 16) |
           (((unsigned)byte_at(word, 2)) << 8) | byte_at(word, 3);
}
