/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/ut/ut_LinkList.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select all four located erase/insert methods under middleware; retain the unlinked reverse definition inline and select release assertions. Algorithms unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
// Filename: ut_LinkList.cpp
//
// Project: NintendoWare4Ctr

#include <nw/ut/ut_LinkList.h>

namespace nw {
namespace ut {
namespace internal {

LinkListImpl::iterator LinkListImpl::erase(iterator it)
{
    iterator itNext = it;
    (void)++itNext;
    return erase(it,itNext);
}

LinkListImpl::iterator LinkListImpl::erase(iterator itFirst,iterator itLast)
{
    Node* pIt = itFirst.m_Pointer;
    Node* const pItLast = itLast.m_Pointer;
    Node* pNext;
    
    for (; pIt != pItLast ; pIt = pNext)
    {
        pNext = pIt->m_Next;
        (void)erase(pIt);
    }

    return itLast;
}

inline void LinkListImpl::reverse(void)
{
    if (empty())
    { 
        return; 
    }
    
    Node* p = m_BaseNode.m_Next;
    Node* pNext;
    
    Initialize_();
    
    for (; p != &this->m_BaseNode ; p = pNext)
    {
        pNext = p->m_Next;
        p->m_Next = NULL;
        p->m_Prev = NULL;
        push_front(p);
    }
}

LinkListImpl::iterator LinkListImpl::insert(iterator it, pointer p)
{
    Node *const pIt = it.m_Pointer;

    Node *const pItPrev = pIt->m_Prev;

    p->m_Next = pIt;
    p->m_Prev = pItPrev;

    pIt->m_Prev = p;
    pItPrev->m_Next = p;

    ++this->m_Size;
    return iterator(p);
}

LinkListImpl::iterator LinkListImpl::erase(pointer p)
{
    Node* const pNext = p->m_Next;
    Node* const pPrev = p->m_Prev;
    
    pNext->m_Prev = pPrev;
    pPrev->m_Next = pNext;
    --m_Size;

    p->m_Next = NULL;
    p->m_Prev = NULL;
    
    return iterator(pNext);
}

}
}
}

