/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_FontContainer.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_FontContainer.cpp
//
// Project: NintendoWare4Ctr

#include <nw/font/font_ResFont.h>
#include <nw/lyt/lyt_FontContainer.h>
#include <nw/lyt/lyt_Layout.h>

#define stricmp std::strcasecmp

namespace nw{
namespace lyt{

/* FontRefLink */







/* FontContainer */





font::Font* FontContainer::FindFontByName(const char* name)
{
    for (Iterator it = this->GetBeginIter(); it != this->GetEndIter(); ++it)
    {
        if (0 == std::strcmp(name, it->GetFontName()))
        {
            return it->GetFont();
        }
    }

    return NULL;
}





}
}

