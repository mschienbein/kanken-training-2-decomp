/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/gfx/res/gfx_ResTextureMapper.cpp, SetupTexture.
 * Native wrappers and std::pair replace the earlier local ABI approximations.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTexture.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>
#include <utility>

/* INTEGRATOR: move to include/game/texture_resource.h. GetReferenceTextureTarget
 * identity is [INFERENCE]; original target and native pair ABI are witnessed
 * by this natural caller. No mangled alias is proposed. */
extern "C" {
nw::gfx::Result func_001c74e8(nw::gfx::res::ResTexture *, nw::os::IAllocator *,
    nw::gfx::res::ResGraphicsFile);
std::pair<nw::gfx::res::ResTexture, bool> func_001c7fa0(
    nw::gfx::res::ResReferenceTexture, nw::gfx::res::ResGraphicsFile);
}

extern "C" nw::gfx::Result func_001c7760(nw::os::IAllocator *allocator,
    nw::gfx::res::ResTexture texture, nw::gfx::res::ResGraphicsFile graphicsFile)
{
    using namespace nw::gfx;
    Result result = RESOURCE_RESULT_OK;
    ResTexture setupTexture;
    bool exists = false;
    if (texture.IsValid()) {
        ResReferenceTexture reference = ResDynamicCast<ResReferenceTexture>(texture);
        if (reference.IsValid()) {
            if (reference.GetTargetTexture().IsValid()) {
                setupTexture = reference.GetTargetTexture();
                exists = true;
            } else {
                std::pair<ResTexture, bool> resolution;
                resolution = func_001c7fa0(reference, graphicsFile);
                if (resolution.second) {
                    setupTexture = resolution.first;
                    exists = true;
                } else {
                    result |= Result::MASK_FAIL_BIT;
                    result |= RESOURCE_RESULT_NOT_FOUND_TEXTURE;
                }
            }
        } else {
            setupTexture = texture;
            exists = true;
        }
    }
    if (exists)
        result |= func_001c74e8(&setupTexture, allocator, graphicsFile);
    return result;
}
