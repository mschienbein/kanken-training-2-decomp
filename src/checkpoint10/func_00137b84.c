#include "game/graphics_state.h"

#include "game/framebuffer.h"
extern int func_0013ed44(const struct GraphicsMutex *session, struct DisplayFramebufferPair *output);

void func_00137b84(struct DisplayFramebufferPair *output)
{
    unsigned format;
    /* ORIGINAL BUG: the query result is ignored before the output is normalized. */
    func_0013ed44(func_001861d4(), output);
    format = output->display[0].format;
    if ((format & 0x70) == 0) {
        output->display[0].format = ~0u;
    } else {
        if ((format & 0x20) == 0)
            output->display[0].right = output->display[0].left;
        output->display[0].format = format & 7;
    }
    output->display[1].format &= 7;
    if (output->display[0].left == 0)
        output->display[0].format = ~0u;
    if (output->display[0].right == 0)
        output->display[0].format = ~0u;
    if (output->display[1].left == 0)
        output->display[1].format = ~0u;
    output->display[1].right = output->display[1].left;
}
