/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResTextureMapper.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select assigned definitions; use canonical curated headers,
 * evidenced numeric allocation selectors, and address-bound retained calls.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTextureMapper.h>
#include <game/render_state.h>
namespace nw { namespace gfx { namespace res {
/* INTEGRATOR: move the existing src/wave2/func_0018f0ac.c contract
 * to include/game/texture_resource.h. */
extern "C" { unsigned func_0018f0ac(const void *address); }
/* INTEGRATOR: differs from include/nw/gfx/res/gfx_ResTexture.h:
 * assigned target accesses the six cube image offsets at 0x38, whereas the
 * pinned public ResCubeTextureData::toImagesTables starts at 0x50. This is a
 * partial target-version view, not a recovered complete serialized type. */
struct CubeImageSlotsR41268 : ResPixelBasedTextureData {
    nw::ut::Offset toImages[ResCubeTexture::MAX_CUBE_FACE];
};
static inline ResPixelBasedImage TargetCubeImage(ResCubeTexture texture,
    ResCubeTexture::CubeFace face)
{
    CubeImageSlotsR41268 *data = reinterpret_cast<CubeImageSlotsR41268 *>(texture.ptr());
    return ResPixelBasedImage(data->toImages[face].to_ptr());
}
enum
{
    HEIGHT_SHIFT = 0,
    WIDTH_SHIFT = 16,
    HEIGHT_MASK = 0x7FF << HEIGHT_SHIFT,
    WIDTH_MASK = 0x7FF << WIDTH_SHIFT,
    ADDRESS_MASK = 0x0FFFFFFF,
    CUBE_ADDRESS_MASK = 0x003FFFFF,
    FORMAT_SHIFT = 0,
    FORMAT_MASK = 0xF,
    FORMAT_ETC_SHIFT = 4,
    FORMAT_ETC_MASK = 0x3 << FORMAT_ETC_SHIFT,
    MIPMAP_SIZE_SHIFT = 16,
    MIPMAP_SIZE_MASK = 0xF << MIPMAP_SIZE_SHIFT,
    SAMPLER_TYPE_SHADOW_2D = 2,
    SAMPLER_TYPE_SHIFT = 28,
    SAMPLER_TYPE_MASK = 0x7 << SAMPLER_TYPE_SHIFT
};
void SetupTextureMapperCommand(ResPixelBasedTextureMapper mapper, ResCubeTexture texture)
{
    u32* command = mapper.GetCommandCache();
    mapper.SetCommandSizeToSend(14 * sizeof(u32));
    
    uint texId = texture.GetTextureObject();
    
    uint format = texture.GetFormatHW();

    command[0] &= ~FORMAT_MASK;
    command[3] &= ~0x0FF00000;
    command[4] &= ~(HEIGHT_MASK | WIDTH_MASK);
    command[5] &= ~(FORMAT_ETC_MASK);
    command[6] &= ~(MIPMAP_SIZE_MASK);
    command[7] &= ~ADDRESS_MASK;
    command[8] &= ~CUBE_ADDRESS_MASK;
    command[9] &= ~CUBE_ADDRESS_MASK;
    command[10] &= ~CUBE_ADDRESS_MASK;
    command[11] &= ~CUBE_ADDRESS_MASK;
    command[12] &= ~CUBE_ADDRESS_MASK;
    
    command[0] |= format & 0xF;

    command[3] |= 9 << 20;
    
    command[4] |= (texture.GetHeight() & 0x7FF) << 0;
    command[4] |= (texture.GetWidth() & 0x7FF) << WIDTH_SHIFT;

    if (format == ResImageTexture::FORMAT_HW_ETC1)
    {
        command[5] |= 2 << FORMAT_ETC_SHIFT;
    }
    
    /* ORIGINAL BUG: absent image/sampler references are unchecked in release. */
    NW_ASSERT(VerifyCubeTextureAddress_(texture));

    ResTextureSampler sampler = mapper.GetSampler();
    NW_ASSERT(sampler.IsValid());
    if ((sampler.GetMinFilter() != ResTextureSampler::MINFILTER_NEAREST) &&
        (sampler.GetMinFilter() != ResTextureSampler::MINFILTER_LINEAR))
        {
        command[6] |= ((texture.GetMipmapSize() - 1) & 0xF) << MIPMAP_SIZE_SHIFT;
    }
    
    for (int face = 0; face < ResCubeTexture::MAX_CUBE_FACE; ++face)
    {
        ResPixelBasedImage resImage = TargetCubeImage(texture, static_cast<ResCubeTexture::CubeFace>(face));
        u32 addr = resImage.GetImageAddress();
        
        if (face == 0)
        {
            command[7] |= (func_0018f0ac(reinterpret_cast<const void *>(addr)) >> 3) & ADDRESS_MASK;
        }
        else{
            command[7 + face] |= (func_0018f0ac(reinterpret_cast<const void *>(addr)) >> 3) & CUBE_ADDRESS_MASK;
        }
    }
}
} } }
