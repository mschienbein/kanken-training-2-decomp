#include "game/sound_runtime.h"

static __forceinline unsigned int align_word(unsigned int size)
{
    return (size + 3) & ~3u;
}

unsigned int func_00180768(struct FreeListPool *pool, void *storage,
                          unsigned int size, unsigned int element_size)
{
    unsigned int aligned = align_word((unsigned int)storage);
    unsigned int stride = align_word(element_size);
    /* ORIGINAL BUG: a byte count smaller than the alignment prefix underflows. */
    unsigned int count = (size - (aligned - (unsigned int)storage)) / stride;
    unsigned int i;
    for (i = 0; i < count; ++i) {
        struct FreeLink *node = (struct FreeLink *)aligned;
        node->next = pool->head;
        pool->head = node;
        aligned += stride;
    }
    return count;
}
