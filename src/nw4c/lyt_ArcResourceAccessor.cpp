/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_ArcResourceAccessor.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the three located methods and retain their used narrow-to-wide copy helper inline; remove unused shader/resource includes. Algorithms unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/lyt/lyt_ArcResourceAccessor.h>

namespace nw { namespace lyt {
namespace {
inline size_t strncpy(wchar_t* dest,std::size_t destCount,const char* src)
{
    --destCount;
    size_t length = 0;
    while (length < destCount && *src != '\0')
    {
        *dest = *src;
        ++dest;
        ++src;
        ++length;
    }

    *dest = L'\0';

    return length;
}
}

ArcResourceAccessor::ArcResourceAccessor():   
    m_ArcBuf(0)
{
}

bool ArcResourceAccessor::Attach(void* archiveStart,const char* resourceRootDirectory)
{
    bool bSuccess = ARCInitHandle(archiveStart, &this->m_ArcHandle);
    if (!bSuccess)
    {
        return false;
    }

    m_ArcBuf = archiveStart;

    const int dstBufCount = sizeof(m_ResRootDir) / sizeof(m_ResRootDir[0]);
    strncpy(this->m_ResRootDir, dstBufCount, resourceRootDirectory);

    return true;
}
font::Font* ArcResourceAccessor::GetFont(const char *name)
{
    font::Font* pFont = this->m_FontList.FindFontByName(name);

    if (pFont == NULL)
    {
        pFont = this->LoadFont(name);

        if (pFont != NULL)
        {
            (void)m_FontList.RegistFont(name, pFont, true);
        }
    }

    return pFont;
}

} }
