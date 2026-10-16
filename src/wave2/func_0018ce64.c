#include "author_types.h"

struct SlotBlock *func_0018ce64(void)
{
    struct SlotBlock *block = data_002429f4.allocate
        ? data_002429f4.allocate(0x10000, 0x100, 0, sizeof(*block))
        : 0;
    if (block) {
        block->words[0] = 0;
        block->words[1] = 0;
        block->words[2] = 0;
        block->words[3] = 0;
        block->words[4] = 0;
        block->words[5] = 0;
        block->words[6] = 0;
        block->words[7] = 0;
        block->next = 0;
        block->previous = 0;
    }
    return block;
}
