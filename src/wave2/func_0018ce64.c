#include "game/render_state.h"

struct Framebuffer *func_0018ce64(void)
{
    struct Framebuffer *block = data_002429f4.allocate
        ? data_002429f4.allocate(0x10000, 0x100, 0, sizeof(*block))
        : 0;
    if (block) {
        block->word00 = 0;
        block->word04 = 0;
        block->word08 = 0;
        block->surface = 0;
        block->word10 = 0;
        block->word14 = 0;
        block->word18 = 0;
        block->word1c = 0;
        block->next = 0;
        block->previous = 0;
    }
    return block;
}
