/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_ResourceAccessor.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_ResourceAccessor.cpp
//
// Project: NintendoWare4Ctr

#include <nw/font/font_ResFont.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_Resources.h>
#include <nw/lyt/lyt_Util.h>

namespace nw{
namespace lyt{







font::Font* ResourceAccessor::LoadFont(const char *name)
{
    u32 size = 0;
    void* pFontRes = this->GetResource(res::RESOURCETYPE_FONT, name, &size);
    if (!pFontRes || size == 0)
    {
        return NULL;
    }

    font::ResFont* pResFont = Layout::NewObj<font::ResFont>();
    if (pResFont == NULL)
    {
        return NULL;
    }

    bool bSuccess = pResFont->SetResource(pFontRes);
    if (!bSuccess)
    {
        Layout::DeleteObj(pResFont);
        return NULL;
    }

    {
        const u32 drawBufferSize = font::ResFont::GetDrawBufferSize(pFontRes);
        void* drawBuffer = Layout::AllocMemory(drawBufferSize, 4);
        pResFont->SetDrawBuffer(drawBuffer);
    }

    return pResFont;
}

}
}

