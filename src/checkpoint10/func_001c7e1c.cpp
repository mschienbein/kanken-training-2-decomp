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
/* INTEGRATOR: move to include/game/texture_initialization.h. This typed
 * by-value resource contract supersedes the earlier opaque pointer drafts. */
extern "C" { void func_0016bbdc(u32 texID, ResPixelBasedImage image, u32 flags); }
static inline void SetTexImage2D( u32 texID, ResPixelBasedImage resImage, u32 loadFlag )
{
    /* ORIGINAL BUG: an absent serialized image is unchecked in release. */
    NW_ASSERT(resImage.IsValid());

    if (resImage.GetLocationAddress() != NULL)
    {
        resImage.ref().m_MemoryArea = ResPixelBasedImageData::AREA_NO_MALLOC;
        return;
    }

    if (loadFlag == 0)
    {
        loadFlag = 0x02010000u;
    }

    NW_ASSERT((loadFlag != (0x02010000u)) || resImage.CheckMemoryLocation());
     
    func_0016bbdc(texID, resImage, loadFlag);
}
extern "C" void func_001c7e1c(ResTexture resTex)
{
    ResShadowTexture resSdwTex = ResDynamicCast<ResShadowTexture>( resTex );
    
    /* ORIGINAL BUG: invalid dynamic casts and absent relative images are dereferenced in release. */
    NW_ASSERT( resSdwTex.IsValid() );
    
    if (resSdwTex.GetTextureObject() != 0)
    {
        return;
    }
    
    u32 texID = reinterpret_cast<u32>(resTex.ptr());
    resSdwTex.SetTextureObject(texID);
    
    ResPixelBasedImage resImage = resSdwTex.GetImage();
    
    u32 loadFlag = resSdwTex.GetLocationFlag();
    
    SetTexImage2D(texID, resImage, loadFlag);
}
} } }
