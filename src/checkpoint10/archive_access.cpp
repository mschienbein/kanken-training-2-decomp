/* SPDX-License-Identifier: GPL-3.0-only
 * Narrow-to-wide helper adapted from NintendoWare4Ctr contributors,
 * sources/libraries/lyt/lyt_ArcResourceAccessor.cpp at
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd.
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-22: use the actual installed native accessor member and
 * archive handle; retain the natural narrow-to-wide helper.
 */
#define NW_RELEASE
#include <nw/lyt/lyt_ArcResourceAccessor.h>

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

void *nw::lyt::ArcResourceAccessor::GetResource(ResType type,
    const char *name, u32 *size)
{
    copy_resource_name(m_ResNameWork, name,
                       sizeof(m_ResNameWork) / sizeof(m_ResNameWork[0]));
    return func_001cdb10(&m_ArcHandle, m_ResRootDir, type, m_ResNameWork, size);
}
