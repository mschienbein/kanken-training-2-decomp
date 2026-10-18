/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/gfx/res/gfx_ResTexture.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select assigned definitions; use canonical curated headers,
 * evidenced numeric allocation selectors, and address-bound retained calls.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <nw/gfx/res/gfx_ResTexture.h>
#include <game/render_state.h>
namespace nw { namespace gfx { namespace res {
/* INTEGRATOR: differs from include/nw/gfx/res/gfx_ResTexture.h:
 * the target's six cube image offsets start at 0x38, not the pinned public
 * toImagesTables offset 0x50. This partial target-version wrapper reuses the
 * canonical pixel-texture base and Offset; it does not establish full size. */
struct TargetCubeTextureR41268Data : ResPixelBasedTextureData {
    nw::ut::Offset toImagesTables[ResCubeTexture::MAX_CUBE_FACE];
};
class TargetCubeTextureR41268 : public ResPixelBasedTexture {
public:
    enum { TYPE_INFO = ResCubeTexture::TYPE_INFO };
    enum { MAX_CUBE_FACE = ResCubeTexture::MAX_CUBE_FACE };
    typedef ResCubeTexture::CubeFace CubeFace;
    NW_RES_CTOR_INHERIT(TargetCubeTextureR41268, ResPixelBasedTexture)
    ResPixelBasedImage GetImage(CubeFace face) {
        return ResPixelBasedImage(ref().toImagesTables[face].to_ptr());
    }
};
static void DeleteImage2D( u32 texID, ResPixelBasedImage resImage )
{
    GLuint area = resImage.ref().m_MemoryArea;
    
    if (area != ResPixelBasedImageData::AREA_NO_MALLOC)
    {
        void* address = reinterpret_cast<void*>( resImage.GetLocationAddress() );
        data_002429f8( area, 0x101u, texID, address );
        resImage.SetLocationAddress( static_cast<u32>(NULL) );
    }
}
void ResCubeTexture_Cleanup(ResTexture resTex)
{
    TargetCubeTextureR41268 resCubeTex = ResDynamicCast<TargetCubeTextureR41268>( resTex );
    /* ORIGINAL BUG: invalid dynamic casts and absent relative images are dereferenced in release. */
    NW_ASSERT(resCubeTex.IsValid());
    
    GLuint texID = resCubeTex.GetTextureObject();
    if (texID == 0)
    {
        return;
    }
    
    for (int face = 0; face < TargetCubeTextureR41268::MAX_CUBE_FACE; ++face)
    {
        ResPixelBasedImage resImage = resCubeTex.GetImage(static_cast<TargetCubeTextureR41268::CubeFace>(face));
        DeleteImage2D(texID, resImage);
    }
    
    resCubeTex.SetTextureObject(0);
}
} } }
