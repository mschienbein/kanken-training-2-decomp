/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_HeapBase.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: isolate the recursive lookup using canonical shared
 * HeapBase/LinkList declarations and make the saved current iterator const.
 * Algorithm unchanged. GNU GPL version 3; see LICENSE.
 */
#define NW_RELEASE
#include <nw/ut/ut_HeapBase.h>

namespace nw { namespace ut {

HeapBase* HeapBase::FindContainHeap(HeapList* pList, const void* memBlock)
{
    u32 memBlockAddress = reinterpret_cast<u32>(memBlock);

    for (HeapList::iterator itr = pList->begin(); itr != pList->end();)
    {
        const HeapList::iterator curItr = itr++;
        if (reinterpret_cast<u32>(curItr->m_HeapStart) <= memBlockAddress &&
            reinterpret_cast<u32>(curItr->m_HeapEnd) > memBlockAddress)
        {
            HeapBase* pChildHeapBase = FindContainHeap(&curItr->m_ChildList, memBlock);
            if (pChildHeapBase)
            {
                return pChildHeapBase;
            }
            return &(*curItr);
        }
    }
    return NULL;
}

} }
