#include "game/item_text.h"
unsigned short *func_0015de60(unsigned short *destination, unsigned capacity_bytes,
                             const unsigned short *source, int count)
{
    unsigned short *out = destination;
    unsigned capacity = capacity_bytes / sizeof(*destination);
    /* ORIGINAL BUG: the source is read before either bound is checked. */
    while (*source != 0 && capacity != 0 && count != 0) {
        --capacity;
        --count;
        *out++ = *source++;
    }
    while (capacity != 0 && count != 0) {
        --capacity;
        --count;
        *out++ = 0;
    }
    return destination;
}
