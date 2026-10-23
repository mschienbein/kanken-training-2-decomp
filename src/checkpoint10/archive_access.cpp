/* SPDX-License-Identifier: GPL-3.0-only
 * Narrow-to-wide helper adapted from NintendoWare4Ctr contributors,
 * sources/libraries/lyt/lyt_ArcResourceAccessor.cpp at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: use the project's archive and list views and retain
 * the parameterized narrow-to-wide helper without emitting unrelated vtables.
 */
#include <stddef.h>
#include "game/resource_font.h"
#include "game/pool_list.h"

/* [INFERENCE] The shared non-owning accessor preserves the canonical writable
 * resource result without asserting native ArcResourceAccessor identity. */

static size_t copy_resource_name(wchar_t *destination, const char *source,
                                 size_t capacity)
{
    --capacity;
    size_t length = 0;
    while (length < capacity && *source != '\0') {
        *destination = *source;
        ++destination;
        ++source;
        ++length;
    }
    *destination = L'\0';
    return length;
}

void *func_001ccf20(ArchiveResourceAccessor *accessor, unsigned type,
                         const char *name, unsigned *size)
{
    copy_resource_name(accessor->file_name, name,
                       sizeof(accessor->file_name) / sizeof(accessor->file_name[0]));
    return func_001cdb10(&accessor->archive, accessor->root_path, type,
                         accessor->file_name, size);
}
