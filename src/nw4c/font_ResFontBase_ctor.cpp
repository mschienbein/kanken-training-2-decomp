/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/font/font_ResFontBase.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the constructor and two filter getters under middleware; retain the used filter helper inline and public command-bit macro. Algorithms unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/font/font_ResFontBase.h>
#include <nw/assert.h>

namespace nw { namespace font {
enum
{
    CMD_TEX_NEAREST = 0,
    CMD_TEX_LINEAR  = 1
};

#define NW_FONT_COMMAND_TEX_WRAP_FILTER( magFilter, minFilter )             \
      ( (magFilter) << 1 | (minFilter) << 2 | ( 0/*isETC1*/ ? 2 : 0 ) << 4  \
    | 0/*wrapT*/ << 8 | 0/*wrapS*/ << 12                                    \
    | 0/*minFilter2*/ << 24 )

ResFontBase::ResFontBase():   
    m_pResource(NULL),
    m_pFontInfo(NULL),
    m_pTexObjs(NULL),
    m_LastCharCode(0),
    m_LastGlyphIndex(GLYPH_INDEX_NOT_FOUND)
    {
    this->EnableLinearFilter(true, true);
}

inline void ResFontBase::EnableLinearFilter(bool atSmall,bool atLarge)
{
    const int magFilter = atLarge ? CMD_TEX_LINEAR: CMD_TEX_NEAREST;
    const int minFilter = atSmall ? CMD_TEX_LINEAR: CMD_TEX_NEAREST;
    m_WrapFilter = 
        NW_FONT_COMMAND_TEX_WRAP_FILTER(
            magFilter,
            minFilter);

    if (!this->IsManaging(NULL))
    {
        this->DeleteTextureNames();
        this->GenTextureNames();
    }
}

bool ResFontBase::IsLinearFilterEnableAtSmall() const
{
    return 0 != (m_WrapFilter & (1 << 2));
}

bool ResFontBase::IsLinearFilterEnableAtLarge() const 
{
    return 0 != (m_WrapFilter & (1 << 1));
}

} }
