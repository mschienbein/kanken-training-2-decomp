#include "game/resource_dispatch.h"

void func_001c7f10(nw::gfx::res::ResLookupTable resource)
{
    nw::gfx::res::ResReferenceLookupTable(resource.ptr()).ref().toTargetLut = 0;
}
