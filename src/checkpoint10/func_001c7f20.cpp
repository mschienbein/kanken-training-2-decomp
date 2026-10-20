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
extern "C" void func_001c7f20(ResTexture resTex)
{
    ResShadowTexture resSdwTex = ResDynamicCast<ResShadowTexture>(resTex);
    /* ORIGINAL BUG: invalid dynamic casts and absent relative images are dereferenced in release. */
    NW_ASSERT(resSdwTex.IsValid());
    
    GLuint texID = resSdwTex.GetTextureObject();
    if (texID == 0)
    {
        return;
    }
    
    ResPixelBasedImage resImage = resSdwTex.GetImage();
    DeleteImage2D(texID, resImage);
    resSdwTex.SetTextureObject(0);
}
} } }
