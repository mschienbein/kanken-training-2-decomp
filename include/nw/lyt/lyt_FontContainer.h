/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_FontContainer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain the public class layouts and declarations for standalone selective reuse.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/types.h>

#include <nw/ut/ut_LinkList.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <cstddef>

namespace nw{
namespace lyt{

class FontRefLink
{
public:
    static const int FONTNAMEBUF_MAX = 128;

    FontRefLink();
    ~FontRefLink();
    void Set(const char* name,font::Font* pFont,bool own);
    const char* GetFontName() const {return m_FontName;}
    font::Font* GetFont() const {return m_pFont;}
    
    ut::LinkListNode m_Link;
protected:
    char m_FontName[FONTNAMEBUF_MAX];
    font::Font* m_pFont;
    bool m_Own;
};

class FontContainer : public ut::LinkList<FontRefLink, offsetof(FontRefLink, m_Link)>
{
public:
    ~FontContainer();
    void Finalize();
    FontKey RegistFont(const char* name, font::Font* pFont, bool own);
    void UnregistFont(FontKey key);
    nw::font::Font* FindFontByName(const char* name);
};

}
}

