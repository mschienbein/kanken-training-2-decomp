#include "game/drawing.h"
unsigned int func_001da368(unsigned int capacity)
{
    unsigned int metadata_size;
    unsigned int bitmap_size = (capacity + 7u) >> 3;
    bitmap_size += 3u;
    bitmap_size &= ~3u;
    metadata_size = (40u + 44u * capacity + bitmap_size + 15u) & ~15u;
    return metadata_size + 4u * func_0016a980(capacity);
}
