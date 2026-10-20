#include "game/resource_dispatch.h"

void func_001c8b1c(nw::gfx::res::ResTextureMapper resource)
{
    nw::gfx::res::ResTexture texture = resource.GetTexture();
    if (texture.IsValid())
        texture.Cleanup();
}
