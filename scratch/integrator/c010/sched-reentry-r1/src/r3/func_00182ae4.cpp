#include "game/graphics_resource_abi.h"

void func_00182ae4(nw::gfx::res::ResLookupTable *reference)
{
    nw::gfx::res::ResLookupTableData *resource = reference->ptr();
    if (resource->typeInfo == nw::gfx::res::ResImageLookupTable::TYPE_INFO)
        data_002434e4[0](nw::gfx::res::ResLookupTable(resource));
    else if (resource->typeInfo == nw::gfx::res::ResReferenceLookupTable::TYPE_INFO)
        data_002434e4[1](nw::gfx::res::ResLookupTable(resource));
}
