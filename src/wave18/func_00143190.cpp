#include "game/resource_dispatch.h"

namespace nw { namespace gfx { namespace res {

void ResTextureMapper::Cleanup()
{
    ResTextureMapperData *resource = ptr();
    if (resource->typeInfo == ResPixelBasedTextureMapper::TYPE_INFO)
        data_002437e0[0](ResTextureMapper(resource));
    else if (resource->typeInfo == ResProceduralTextureMapper::TYPE_INFO)
        data_002437e0[1](ResTextureMapper(resource));
}

} } }
