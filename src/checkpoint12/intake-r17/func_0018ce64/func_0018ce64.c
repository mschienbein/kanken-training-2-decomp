#include "game/render_state.h"

struct Framebuffer *func_0018ce64(void)
{
    struct Framebuffer *block = data_002429f4.allocate
        ? data_002429f4.allocate(0x10000, 0x100, 0, sizeof(*block))
        : 0;
    if (block) {
        block->attachments[0].kind = 0;
        block->attachments[0].object_name = 0;
        block->attachments[0].target = 0;
        block->attachments[0].surface = 0;
        block->attachments[1].kind = 0;
        block->attachments[1].object_name = 0;
        block->attachments[1].target = 0;
        block->attachments[1].surface = 0;
        block->next = 0;
        block->previous = 0;
    }
    return block;
}
