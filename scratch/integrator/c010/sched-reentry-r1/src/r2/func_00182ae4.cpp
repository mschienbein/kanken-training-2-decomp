#include "game/graphics_resource_abi.h"

void func_00182ae4(nw::gfx::res::ResLookupTable *reference)
{
    nw::gfx::res::ResLookupTable object = *reference;
    nw::ut::ResTypeInfo kind = object.GetTypeInfo();
    if (kind == nw::gfx::res::ResImageLookupTable::TYPE_INFO)
        data_002434e4[0](object);
    else if (kind == nw::gfx::res::ResReferenceLookupTable::TYPE_INFO)
        data_002434e4[1](object);
}
