#include <preinclude.h>
#include "game/render_context.h"

static __forceinline bool renderAllFlags(unsigned flags, unsigned mask)
{
    mask &= ~flags;
    return mask == 0;
}

void func_00191c94(nw::gfx::RenderContext::MaterialHash *cache, unsigned flags)
{
    const unsigned invalid = ~0u;
    if (renderAllFlags(flags, 0x1)) cache->shaderParameter = invalid;
    if (renderAllFlags(flags, 0x2)) cache->shadingParameter = invalid;
    if (renderAllFlags(flags, 0x4)) cache->materialColor = invalid;
    if (renderAllFlags(flags, 0x8)) cache->rasterization = invalid;
    if (renderAllFlags(flags, 0x10)) cache->textureCoordinator = invalid;
    if (renderAllFlags(flags, 0x20)) cache->textureMapper = invalid;
    if (renderAllFlags(flags, 0x80)) cache->fragmentLighting = invalid;
    if (renderAllFlags(flags, 0x100)) cache->fragmentLightingTable = invalid;
    if (renderAllFlags(flags, 0x200)) cache->textureCombiner = invalid;
    if (renderAllFlags(flags, 0x800)) cache->fragmentOperation = invalid;
    if (renderAllFlags(flags, 0x400)) cache->alphaTest = invalid;
}
