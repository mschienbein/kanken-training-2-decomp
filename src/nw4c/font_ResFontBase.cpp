/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/font/font_ResFontBase.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select three game-profile resource methods, retain the used active-sheet helper inline, and replace SDK release assertion/API includes with shared equivalent release declarations. Algorithms unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/font/font_ResFontBase.h>
#include <nw/font/font_GraphicsApi.h>
#include <nw/assert.h>

namespace nw { namespace font {
void* ResFontBase::RemoveResourceBuffer()
{
    if (NULL != m_pTexObjs)
    {
        DeleteTextureNames();
        this->m_pTexObjs = NULL;
    }

    void* pUserData = m_pResource;

    m_pResource = NULL;
    m_pFontInfo = NULL;

    return pUserData;
}

void ResFontBase::GenTextureNames()
{
    internal::TextureObject* texObjs = GetTextureObjectsBufferPtr();


    const FontTextureGlyph& tg = *m_pFontInfo->pGlyph;
    const int sheetNum = GetActiveSheetNum();
    u32 offsetBytes = 0;
    for (int i = 0; i < sheetNum; ++i)
    {
        const GLuint texName = 0;
        const void *const pImage = tg.sheetImage + offsetBytes;
        const TexFmt format = static_cast<TexFmt>(tg.sheetFormat);
        texObjs[i].Set(texName, this, pImage, format, tg.sheetWidth, tg.sheetHeight);

        offsetBytes += tg.sheetSize;
    }
    nngxUpdateBuffer(tg.sheetImage, tg.sheetSize * sheetNum);  
}

void ResFontBase::DeleteTextureNames()
{
    internal::TextureObject* texObjs = GetTextureObjectsBufferPtr();
    NW_POINTER_ASSERT(texObjs);

    const int sheetNum = GetActiveSheetNum();
    for (int i = 0; i < sheetNum; ++i)
    {
        const GLuint texName = texObjs[i].GetName();
        if (texName != 0)
        {
            glDeleteTextures(1, &texName);
            texObjs[i].SetName(0);
        }

#if defined(NW_DEBUG)
        texObjs[i].Reset();
#endif
    }
}

inline int ResFontBase::GetActiveSheetNum() const
{
    return m_pFontInfo->pGlyph->sheetNum;
}

} }
