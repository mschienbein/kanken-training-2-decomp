/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_InstancePool.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_InstancePool.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_InstancePool.h>
#include <nw/ut.h>

/*

Some can be copied from NW4R since NW4C and NW4R are somewhat simliar.

[REFERENCE] https://github.com/doldecomp/ogws/blob/master/src/nw4r/snd/snd_InstancePool.cpp

*/

namespace nw {
namespace snd {
namespace internal {


void* PoolImpl::AllocImpl() 
{
    if (m_pNext == NULL) 
    {
        return NULL;
    }

    PoolImpl* pHead = m_pNext;
    m_pNext = pHead->m_pNext;

    return pHead;
}


} // namespace internal
} // namespace snd
} // namespace nw

