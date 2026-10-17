/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_HeapBase.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select Initialize/Finalize, retain used helpers inline, and externalize the anonymous root list through a shared typed declaration. Algorithms and fill constants unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/ut/ut_HeapBase.h>
#include <nw/ut/ut_Inlines.h>
#include <cstring>

#define     GetBitValue(data, st, bits) \
    (((data) >>(st)) & ((1 <<(bits)) -1))

#define     SetBitValue(data, st, bits, val)                        \
                do                                                          \
                {                                                           \
                    u32 maskBits = (u32)((1 <<(bits)) -1);                   \
                    u32 newVal = (val) & maskBits;                          \
                    (void)(maskBits <<= st);                                 \
                    (data) &= ~maskBits;                                   \
                    (data) |= newVal <<(st);                                 \
                } while (false);

namespace nw { namespace ut {
namespace {
u32 s_FillVals[HeapBase::HEAP_FILL_MAX] ={
    0xC3C3C3C3,
    0xF3F3F3F3,
    0xD3D3D3D3,
};
}

inline HeapBase::HeapList* HeapBase::FindListContainHeap(HeapBase* pHeapBase)
{
    HeapBase::HeapList* pList = &internal::g_RootList;
    HeapBase* pContainHeapBase = HeapBase::FindContainHeap(&internal::g_RootList, pHeapBase);

    if (pContainHeapBase)
    {
        pList = &pContainHeapBase->m_ChildList;
    }

    return pList;
}

inline u32 HeapBase::GetFillValue(FillType type)
{
    return s_FillVals[type];
}

inline void HeapBase::FillNoUseMemory(void* address, u32 size)
{
    if (this->GetOptionFlag() & OPT_DEBUG_FILL)
    {
        std::memset(address, GetFillValue(HEAP_FILL_NOUSE), size);
    }
}

inline u16 HeapBase::GetOptionFlag()
{
    return (u16)GetBitValue(this->m_Attribute, 0, 8);
}

inline void HeapBase::SetOptionFlag(u16 optFlag)
{
    SetBitValue(this->m_Attribute, 0, 8, optFlag);
}

void HeapBase::Initialize(u32 signature, void* heapStart, void* heapEnd, u16 optFlag)
{
    m_Signature = signature;

    m_HeapStart = heapStart;
    m_HeapEnd = heapEnd;

    m_Attribute = 0;
    SetOptionFlag(optFlag);
    
    FillNoUseMemory(heapStart, (u32)GetOffsetFromPtr(heapStart, heapEnd));

    HeapList* pList = FindListContainHeap(this);
    pList->push_back(this);
}

void HeapBase::Finalize()
{
    HeapList* pList = FindListContainHeap(this);
    pList->erase(this);
    m_Signature = 0;
}

void HeapBase::FillFreeMemory(void* address, u32 size)
{
    if (this->GetOptionFlag() & OPT_DEBUG_FILL)
    {
        std::memset(address, GetFillValue( HEAP_FILL_FREE), size);
    }
}

void HeapBase::FillAllocMemory(void* address, u32 size)
{
    if (this->GetOptionFlag() & OPT_0_CLEAR)
    {
        std::memset(address, 0, size);
    }
    else
    {
        if (this->GetOptionFlag() & OPT_DEBUG_FILL)
        {
            std::memset(address, GetFillValue(HEAP_FILL_ALLOC), size);
        }
    }
}

} }
