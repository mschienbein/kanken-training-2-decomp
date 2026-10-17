/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_FrameHeap.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public class layouts; normalize standalone release include paths.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/ut/ut_HeapBase.h>

namespace nw {
namespace ut {

class FrameHeap : public HeapBase
{
public:
    static const int FREE_HEAD = (1 << 0);
    static const int FREE_TAIL = (1 << 1);
    static const int FREE_ALL = (FREE_HEAD | FREE_TAIL);

private:
    struct HeapState
    {
        u32         tagName;
        void*       headAllocator;
        void*       tailAllocator; 
        HeapState*  pPrevState;

        HeapState():
            tagName(0),
            headAllocator(NULL),
            tailAllocator(NULL),
            pPrevState(NULL) {}
    };

  public:
    static  FrameHeap* Create(void* startAddress, u32 heapSize, u16 optFlag = 0);

    void* Destroy();
    void* Alloc(u32 size, int alignment = DEFAULT_ALIGNMENT);
    void  Free(int mode);
    bool  RecordState(u32 tagName);
    bool  FreeByState(u32 tagName);
  private:
    bool IsValid() { return GetSignature() == FRMHEAP_SIGNATURE; }
    void* AllocFromHead(u32 size, int alignment);
    void* AllocFromTail(u32 size, int alignment);
    void FreeHead();
    void FreeTail();

    void* m_HeadAllocator;
    void* m_TailAllocator;
    HeapState* m_pState;
};


}
}

