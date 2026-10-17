#include "game/runtime.h"

unsigned char *func_00152d48(struct BitmapPool *pool)
{
    int byte_index;
    int bit_index;
    int byte_count;
    unsigned char mask;
    if (pool->capacity > pool->used) {
        byte_count = ((pool->capacity + 7) & ~7) / 8;
        for (byte_index = 0; byte_index < byte_count; ++byte_index) {
            unsigned char byte = pool->bits[byte_index];
            if (byte != 0xff) {
                mask = 1;
                for (bit_index = 0; bit_index < 8; ++bit_index) {
                    if (byte & mask) {
                        mask <<= 1;
                    } else {
                        unsigned int slot = bit_index + byte_index * 8;
                        unsigned int stride;
                        unsigned char *allocation;
                        pool->bits[byte_index] |= mask;
                        ++pool->used;
                        stride = pool->stride;
                        allocation = pool->storage;
                        /* ORIGINAL BUG: rounded-up bitmap capacity can expose a padding bit. */
                        return allocation + stride * slot;
                    }
                }
            }
        }
    }
    return 0;
}
