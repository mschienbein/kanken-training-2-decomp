/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_ThreadStack.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public class layouts; normalize standalone release include paths.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_THREAD_STACK_H_
#define NW_SND_THREAD_STACK_H_

#include <nw/types.h>

namespace nw {
namespace snd {
namespace internal {

class ThreadStack
{
public:
    ThreadStack(): 
        m_Addr(NULL), m_Size(0) 
    {
    }
    ~ThreadStack() { Finalize(); }

    void Initialize( uptr ptr, size_t size );
    void Finalize();

    uptr GetStackBottom() const { return m_Addr + m_Size; }
    uptr GetBaseAddress() const { return m_Addr; }

private:
    uptr    m_Addr;
    size_t  m_Size;
};

} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_THREAD_STACK_H_

