#include "game/resource_dispatch.h"
#include <nw/gfx/gfx_GraphicsDevice.h>

void func_001c6a34(nw::gfx::res::ResLookupTable resource)
{
    nw::gfx::GraphicsDevice::InvalidateLookupTable(
        nw::gfx::res::ResImageLookupTable(resource.ptr()));
}
