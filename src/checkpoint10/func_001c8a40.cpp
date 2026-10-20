/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr gfx_ResTextureMapper.cpp,
 * ctrdecomp/NintendoWare4Ctr commit 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified: address-named interfaces; canonical native resources retained.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>
#include <nw/gfx/res/gfx_ResGraphicsFile.h>

using namespace nw::gfx;

/* INTEGRATOR: move to include/game/texture_mapper.h; canonical native views
 * replace the earlier inferred TextureCommandBlock/Environment/Package types. */
extern "C" {
Result func_001c7760(nw::os::IAllocator *allocator, ResTexture texture, ResGraphicsFile file);
void func_001c8244(ResPixelBasedTextureMapper mapper, ResCubeTexture texture);
void func_001c8424(ResPixelBasedTextureMapper mapper, ResImageTexture texture);
void func_001c857c(ResPixelBasedTextureMapper mapper, ResShadowTexture texture);
}

extern "C" Result func_001c8a40(ResTextureMapper mapper, nw::os::IAllocator *allocator, ResGraphicsFile file)
{
    ResPixelBasedTextureMapper sampled = nw::ut::ResStaticCast<ResPixelBasedTextureMapper>(mapper);
    ResTexture texture = mapper.GetTexture();
    Result result = func_001c7760(allocator, texture, file);
    if (result.IsFailure())
        return result;
    /* ORIGINAL BUG: successful setup of a null texture is followed by dereference. */
    ResTexture image = texture.Dereference();
    switch (image.ref().typeInfo) {
    case ResImageTexture::TYPE_INFO:
        func_001c8424(sampled, nw::ut::ResStaticCast<ResImageTexture>(image));
        break;
    case ResCubeTexture::TYPE_INFO:
        func_001c8244(sampled, nw::ut::ResStaticCast<ResCubeTexture>(image));
        break;
    case ResShadowTexture::TYPE_INFO:
        func_001c857c(sampled, nw::ut::ResStaticCast<ResShadowTexture>(image));
        break;
    }
    return result;
}
