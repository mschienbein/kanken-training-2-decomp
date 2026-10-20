#include "game/resource_dispatch.h"

namespace nw { namespace gfx { namespace res {

void ResTexture::Cleanup()
{
    ResTextureData *resource = ptr();
    int type = resource->typeInfo;
    switch (type) {
    case ResImageTexture::TYPE_INFO:
        data_00243510[0](ResTexture(resource));
        break;
    case ResCubeTexture::TYPE_INFO:
        data_00243510[1](ResTexture(resource));
        break;
    case ResReferenceTexture::TYPE_INFO:
        data_00243510[2](ResTexture(resource));
        break;
    case ResProceduralTexture_TYPE_INFO:
        data_00243510[3](ResTexture(resource));
        break;
    case ResShadowTexture::TYPE_INFO:
        data_00243510[4](ResTexture(resource));
        break;
    }
}

} } }
