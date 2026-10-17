#include "game/framebuffer.h"

/* Matches the RenderTargetVTable::handle contract in func_0019f170.cpp. */
extern "C" const unsigned *func_001f0174(FramebufferRenderTarget *target)
{
    return &target->binding.handle;
}
