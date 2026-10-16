#include "game/render_state.h"

struct FramebufferBinding {
    unsigned handle;
    int height;
    int width;
    unsigned color_format;
    unsigned depth_format;
    void *color_memory;
    void *depth_memory;
};


extern "C" void func_001245b0(FramebufferBinding *binding, unsigned handle)
{
    binding->handle = handle;
    if (handle != 0)
        func_00130e24(binding->handle, &binding->color_memory, &binding->depth_memory);
}
