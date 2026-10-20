/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr contributors, io_FileStream.cpp at
 * https://github.com/ctrdecomp/NintendoWare4Ctr/blob/15ddb5632db5a6f0d2f46e3827fc78df468684bd/sources/libraries/io/io_FileStream.cpp
 */
#define NW_RELEASE
#include <nw/io/io_FileStream.h>
#include <nw/ut/ut_Inlines.h>

namespace nw { namespace io {
u32 FileStream::FilePosition::Skip(s32 offset)
{
    if (offset != 0) {
        /* ORIGINAL BUG: unsigned addition wraps before the 64-bit clamp. */
        s64 position = m_Position + offset;
        position = ut::Clamp(position, static_cast<s64>(0), static_cast<s64>(m_FileSize));
        m_Position = static_cast<u32>(position);
    }
    return m_Position;
}
} }
