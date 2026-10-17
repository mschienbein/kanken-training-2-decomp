/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_LinkList.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the new range-erase method under middleware; methods collected in earlier checkpoints remain declaration-only. Omit the unused reverse definition and select release assertions. Retained algorithm unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
/* [INFERENCE] Pre-existing method bodies are retained inline so that the
 * compiler can reproduce their inlining inside the new methods. Their
 * out-of-line entries remain owned by earlier checkpoints; c007 does not
 * credit another definition of those addresses. Inline placement is a
 * disclosed source/codegen boundary, not recovered original spelling. */
#define NW_RELEASE
// Filename: ut_LinkList.cpp
//
// Project: NintendoWare4Ctr

#include <nw/ut/ut_LinkList.h>

namespace nw {
namespace ut {
namespace internal {


LinkListImpl::iterator LinkListImpl::erase(iterator itFirst, iterator itLast)
{
    Node *pIt = itFirst.m_Pointer;
    Node *const pItLast = itLast.m_Pointer;
    while (pIt != pItLast)
    {
        Node *const pNext = pIt->m_Next;
        Node *const pPrev = pIt->m_Prev;
        pNext->m_Prev = pPrev;
        pPrev->m_Next = pNext;
        --m_Size;
        pIt->m_Next = NULL;
        pIt->m_Prev = NULL;
        pIt = pNext;
    }
    return itLast;
}

}
}
}

