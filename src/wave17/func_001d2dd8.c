#include <stdint.h>

struct LinearAllocator {
    unsigned char reserved00[0x10];
    unsigned char *end;
    unsigned char *current;
};

void *func_001d2dd8(struct LinearAllocator *allocator, unsigned size)
{
    unsigned char *current = allocator->current;
    unsigned char *next = current + size;
    if (allocator->end < next)
        return 0;
    allocator->current = (unsigned char *)(((uintptr_t)next + 31u) & ~31u);
    return current;
}
