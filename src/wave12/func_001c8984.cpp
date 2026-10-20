#include "game/resource_dispatch.h"

void func_001c8984(nw::gfx::res::ResTexture resource)
{
    nw::gfx::res::ResReferenceTexture(resource.ptr()).ref().toTargetTexture = 0;
}
