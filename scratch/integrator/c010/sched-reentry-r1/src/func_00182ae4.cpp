#include "game/graphics_resource_abi.h"

void func_00182ae4(nw::gfx::res::ResLookupTable *reference)
{
    if (reference->GetTypeInfo() == nw::gfx::res::ResImageLookupTable::TYPE_INFO)
        data_002434e4[0](*reference);
    else if (reference->GetTypeInfo() == nw::gfx::res::ResReferenceLookupTable::TYPE_INFO)
        data_002434e4[1](*reference);
}
