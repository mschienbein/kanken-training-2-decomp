/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/gfx/res/gfx_ResTexture.cpp, ResTexture::Setup.
 * Uses the canonical native resource wrapper and by-value callback contract.
 */
#include "game/texture_resource.h"

extern "C" nw::gfx::Result func_001c74e8(nw::gfx::res::ResTexture *texture,
    nw::os::IAllocator *allocator, nw::gfx::res::ResGraphicsFile graphicsFile)
{
    using namespace nw::gfx;
    Result result = RESOURCE_RESULT_OK;
    switch (texture->ref().typeInfo) {
    case ResImageTexture::TYPE_INFO: {
        data_002434fc[0](*texture);
        }
        goto done;
    case ResCubeTexture::TYPE_INFO: {
        data_002434fc[1](*texture);
        }
        goto done;
    case ResReferenceTexture::TYPE_INFO: {
        data_002434fc[2](*texture);
        }
        goto done;
    case ResProceduralTexture_TYPE_INFO: {
        data_002434fc[3](*texture);
        }
        goto done;
    case ResShadowTexture::TYPE_INFO: {
        data_002434fc[4](*texture);
        }
        goto done;
    default: {
        }
    }
done:
    return result;
}
