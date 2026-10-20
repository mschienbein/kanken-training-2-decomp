#include "game/graphics_resource_abi.h"
#include <nw/gfx/gfx_GraphicsDevice.h>

/* The shared dispatcher's image type tag establishes the payload projection;
 * the provider itself takes the same native value as its callback slot. */
void func_001c6a34(nw::gfx::res::ResLookupTable object)
{
    nw::gfx::GraphicsDevice::InvalidateLookupTable(
        nw::gfx::res::ResImageLookupTable(object.ptr()));
}
