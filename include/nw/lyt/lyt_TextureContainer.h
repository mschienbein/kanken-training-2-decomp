/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/lyt/lyt_TextureContainer.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain the public class layouts and declarations for standalone selective reuse.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/ut/ut_LinkList.h>
#include <nw/lyt/lyt_Types.h>

namespace nw{
namespace lyt{

class TextureRefLink
{
public:
    static const int TEXIMAGE_FILENAME_MAX = 127;

    ut::LinkListNode m_Link;

    TextureRefLink();
    ~TextureRefLink();
    void Set(const char* name,const TextureInfo& texInfo);

    const char* GetResourceName() const { return m_Name; }
    const TextureInfo GetTextureInfo() {return m_TexInfo; }

protected:
    char m_Name[TEXIMAGE_FILENAME_MAX + 1];
    TextureInfo m_TexInfo;
};

class TextureContainer : public ut::LinkList<TextureRefLink, offsetof(TextureRefLink, m_Link)>
{
public:
    ~TextureContainer();
    void Finalize();
    TextureKey RegistTexture(const char* name, const TextureInfo& textureInfo);
    void UnregistTexture(TextureKey key);
    const TextureInfo FindTextureByName(const char* name);
    const TextureInfo FindTextureByKey(TextureKey key);
};

}
}

