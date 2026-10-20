#include "game/resource_dispatch.h"

namespace nw { namespace gfx { namespace res {

void ResLookupTable::Cleanup()
{
    ResLookupTableData *resource = ptr();
    if (resource->typeInfo == ResImageLookupTable::TYPE_INFO)
        data_002434e4[0](ResLookupTable(resource));
    else if (resource->typeInfo == ResReferenceLookupTable::TYPE_INFO)
        data_002434e4[1](ResLookupTable(resource));
}

} } }
