/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr contributors at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain assigned native definitions and shared helpers;
 * use canonical headers and explicit non-owning views for cleanup contracts.
 * [INFERENCE] Historical TU identity remains unproved; the split follows the
 * public library's separate FontContainer, TextureContainer and Animation units.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_TextureContainer.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Util.h>
#include "game/functions.h"
#include "game/font_resource.h"
#include "game/shader.h"

struct TextureCacheNode {
    PoolLink link;
    char name[128];
    nw::lyt::TextureInfo texture;
};


namespace nw { namespace lyt {
namespace {
static void copy_resource_name(char *destination, std::size_t capacity,
                               const char *source)
{
    // ORIGINAL BUG: the shared NW4C helper tests the next character after
    // copying the first one, so an initially empty name is read past its NUL.
    ut::strcpy(destination, capacity, source);
}

static PoolLink *pop_front(PoolLink *const &head, PoolList &list)
{
    PoolLink *entry = head;
    func_0018d868(&list, entry);
    return entry;
}
}

inline TextureRefLink::TextureRefLink()
{
    m_Name[0] = '\0';
}

inline void TextureRefLink::Set(const char *name, const TextureInfo &texture)
{
    copy_resource_name(m_Name, sizeof(m_Name), name);
    m_TexInfo = texture;
}

TextureKey TextureContainer::RegistTexture(const char *name,
                                           const TextureInfo &texture)
{
    TextureRefLink *entry = Layout::NewObj<TextureRefLink>();
    if (entry == 0)
        return 0;
    entry->Set(name, texture);
    push_back(entry);
    return reinterpret_cast<TextureKey>(entry);
}

extern "C" PoolList *func_001cc4cc(PoolList *list)
{
    for (;;) {
        if (list->count == 0)
            return list;
        TextureCacheNode *entry = reinterpret_cast<TextureCacheNode *>(
            pop_front(list->sentinel.next, *list));
        if (entry != 0) {
            DeleteTexture(entry->texture);
            func_00191aec(entry);
        }
    }
}

} }
