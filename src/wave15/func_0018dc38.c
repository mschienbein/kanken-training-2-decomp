#include "game/render_state.h"

struct Framebuffer;

extern void func_00140fd8(int width, int height, unsigned format, unsigned type,
                          unsigned levels, unsigned parameter, struct Renderbuffer *renderbuffer);

void func_0018dc38(unsigned target, unsigned format, int width, int height)
{
    struct GpuState *state = data_00242fd0;
    struct Renderbuffer *renderbuffer;
    unsigned module = target & 0xffff0000;
    if (module == 0)
        module = 0x20000;
    else if (module != 0x20000 && module != 0x30000)
        return;
    /* ORIGINAL BUG: storage requires a bound renderbuffer and does not check for null. */
    renderbuffer = data_002426d8->bound_renderbuffer;
    if (renderbuffer->memory) {
        if (renderbuffer->format == format && renderbuffer->width == width &&
            renderbuffer->height == height)
            return;
        if (data_002429f8)
            data_002429f8(renderbuffer->module, 0x103, renderbuffer->handle,
                          renderbuffer->memory);
        renderbuffer->memory = 0;
    }
    func_00140fd8(width, height, format, 0, 1, 0, renderbuffer);
    renderbuffer->memory = data_002429f4.allocate
        ? data_002429f4.allocate(module, 0x103, renderbuffer->handle, renderbuffer->size)
        : 0;
    renderbuffer->module = module;
    state->dirty |= 1;
}
