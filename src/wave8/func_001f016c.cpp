#include "game/framebuffer.h"

extern "C" const RenderTargetConfiguration *func_001f016c(
    const FramebufferRenderTarget *target)
{
    return &target->configuration;
}
