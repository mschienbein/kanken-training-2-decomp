/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_FrameHeap.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select FreeByState and externalize its release hooks.
 * [INFERENCE] armlink-inlined empty release lock/unlock hooks were removed by
 * --remove; their historical identity, existence and original address are
 * not proved. Only the separate opt-in v3 model admits these unbound hooks.
 * This file is free software under GNU GPL version 3; see src/nw4c/LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/ut/ut_FrameHeap.h>
#include <nw/ut/ut_Inlines.h>

#include "game/region_heap.h"

namespace nw {
namespace ut {

bool FrameHeap::FreeByState(u32 tagName)
{
    bool retVal;
    frame_heap_release_lock(this);
    {
        HeapState* pState = m_pState;
        if (tagName != 0)
        {
            for (; pState; pState = pState->pPrevState)
            {
                if (pState->tagName == tagName)
                    break;
            }
        }
        if (pState == NULL)
        {
            retVal = false;
        }
        else
        {
            void* oldHeadAllocator = m_HeadAllocator;
            void* oldTailAllocator = m_TailAllocator;
            m_HeadAllocator = pState->headAllocator;
            m_TailAllocator = pState->tailAllocator;
            m_pState = pState->pPrevState;
            FillFreeMemory(m_HeadAllocator, (u32)GetOffsetFromPtr(this->m_HeadAllocator, oldHeadAllocator));
            FillFreeMemory(oldTailAllocator, (u32)GetOffsetFromPtr(oldTailAllocator, this->m_TailAllocator));
            retVal = true;
        }
    }
    frame_heap_release_unlock(this);
    return retVal;
}

}
}
