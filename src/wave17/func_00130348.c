#include <stdint.h>
#include "game/linear_buffer.h"


void func_00130348(struct AlignedBuffer *buffer, unsigned char *memory,
                   unsigned size, unsigned char enabled)
{
    unsigned char *begin = (unsigned char *)(((uintptr_t)memory + 3) & ~(uintptr_t)3);
    size = (unsigned)(memory + size - begin);
    buffer->begin = begin;
    buffer->enabled = enabled;
    buffer->used = 0;
    buffer->size = size & ~7u;
}
