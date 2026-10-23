/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; sources/libraries/lyt/lyt_ArcResourceAccessor.cpp
 * Pinned upstream: 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * Modified 2026-10-20: select aggregate GetTexture; keep the second, unrecovered
 * owner distinct instead of aliasing it to ArcResourceAccessor.
 * GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#include <preinclude.h>
#include <cstring>
#include <nw/lyt/lyt_ArcResourceAccessor.h>

namespace nw { namespace lyt {
const TextureInfo ArcResourceAccessor::GetTexture(const char* name)
{
    TextureInfo texture = m_TextureList.FindTextureByName(name);
    if (!texture.IsValid()) {
        const nw::lyt::TextureInfo loaded = LoadTexture(name);
        std::memcpy(&texture, &loaded, sizeof(texture));
        if (texture.IsValid())
            (void)m_TextureList.RegistTexture(name, texture);
    }
    return texture;
}
} }

/* [INFERENCE] This is only a diagnostic target owner for the independently
 * observed cache at +0x1c. Neither its nominal native identity nor its other
 * virtual endpoints are recovered; no source credit is claimed for this view. */
class TextureCacheAccessor28 : public nw::lyt::ResourceAccessor {
public:
    virtual ~TextureCacheAccessor28();
    virtual const nw::lyt::TextureInfo GetTexture(const char* name);
private:
    unsigned char reserved04[0x18];
    nw::lyt::TextureContainer m_TextureList;
};

const nw::lyt::TextureInfo TextureCacheAccessor28::GetTexture(const char* name)
{
    nw::lyt::TextureInfo texture = m_TextureList.FindTextureByName(name);
    if (!texture.IsValid()) {
        const nw::lyt::TextureInfo loaded = LoadTexture(name);
        std::memcpy(&texture, &loaded, sizeof(texture));
        if (texture.IsValid())
            (void)m_TextureList.RegistTexture(name, texture);
    }
    return texture;
}
