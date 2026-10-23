/* SPDX-License-Identifier: GPL-3.0-only
 * The bounded narrow-to-wide helper follows NintendoWare4Ctr's
 * lyt_ArcResourceAccessor.cpp at 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * The enclosing named-archive view is inferred from this caller's accesses.
 */
#define NW_RELEASE
#include <nw/lyt/lyt_Arc.h>
#include <cstddef>

#include "game/resource_font.h"

static std::size_t copy_archive_name(wchar_t *destination, std::size_t capacity,
                                   const char *source)
{
    --capacity;
    std::size_t length = 0;
    while (length < capacity && *source != '\0') {
        *destination = *source;
        ++destination;
        ++source;
        ++length;
    }
    *destination = L'\0';
    return length;
}


bool func_00140490(NamedArchive *archive, void *file, const char *name)
{
    if (!nw::lyt::ARCInitHandle(file, &archive->view))
        return false;
    copy_archive_name(archive->name, 64, name);
    return true;
}
