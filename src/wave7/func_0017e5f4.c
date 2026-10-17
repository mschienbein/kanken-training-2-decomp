#include "game/runtime.h"

static __forceinline void clear_slot(unsigned int index, struct BitmapPool *pool)
{
    unsigned char *byte = &pool->bits[index >> 3];
    index &= 7;
    *byte &= ~(1u << index);
}

void func_0017e5f4(struct BitmapPool *pool, const void *element)
{
    unsigned int offset = (unsigned int)element - (unsigned int)pool->storage;
    unsigned int index = offset / pool->stride;
    clear_slot(index, pool);
    --pool->used;
}
