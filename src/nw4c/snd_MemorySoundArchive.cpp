/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_MemorySoundArchive.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_MemorySoundArchive.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_MemorySoundArchive.h>
#include <cstring>

namespace nw {
namespace snd {

MemorySoundArchive::MemoryFileStream::MemoryFileStream(const void* buffer, u32 size): 
    m_pBuffer(buffer), 
    m_Size(size), 
    m_Position(0) 
{
}

void MemorySoundArchive::MemoryFileStream::Close() 
{
    m_pBuffer = NULL;
    m_Size = 0;
    m_Position = 0;
}

void MemorySoundArchive::MemoryFileStream::Seek(s32 offset, u32 origin) 
{
    switch (origin) 
    {
    case SEEK_SET: 
    {
        m_Position = offset;
        break;
    }

    case SEEK_CUR: 
    {
        m_Position += offset;
        break;
    }

    case SEEK_END: 
    {
        m_Position = m_Size - offset;
        break;
    }

    default: 
    {
        NW_WARNING("Unsupported Seek origin");
        return;
    }
    }
}

} // namespace snd
} // namespace nw

