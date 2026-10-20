#include "game/render_state.h"


int func_00118074(struct FramebufferManager *list)
{
    struct Framebuffer *block;
    int result;
    data_002426d8 = list;
    block = func_0018ce64();
    if (block) {
        block->next = 0;
        block->previous = 0;
        data_002426d8->framebuffers = block;
        data_002426d8->renderbuffers = 0;
        data_002426d8->bound_framebuffer = block;
        data_002426d8->bound_renderbuffer = 0;
        result = 0;
    } else {
        result = -1;
    }
    return result;
}
