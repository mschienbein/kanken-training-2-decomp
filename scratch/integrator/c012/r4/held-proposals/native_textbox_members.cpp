/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/lyt/lyt_TextBox.cpp.
 * Target-version independent dirty storage stays private in the native owner.
 */
#include "game/textbox.h"
#include <cstring>
#include <nw/ut/ut_Inlines.h>

namespace nw { namespace lyt {

u16 TextBox::SetString(const wchar_t *text, u16 offset)
{
    return SetStringImpl(text, offset, func_0019d5b0(text));
}

u16 TextBox::SetStringImpl(const wchar_t *text, u16 offset, u32 length)
{
    if (!m_pFont)
        return 0;
    if (!m_pTextBuf)
        return 0;
    const u16 capacity = m_TextBufBytes ? static_cast<u16>(m_TextBufBytes / 2 - 1) : 0;
    if (offset >= capacity)
        return 0;
    if (m_TextLen < offset)
        return 0;
    if (length && (!text || !*text))
        return 0;
    unsigned copied = nw::ut::Min(length, unsigned(capacity - offset));
    /* ORIGINAL BUG: zero copy can pass null to memcpy. */
    std::memcpy(m_pTextBuf + offset, text, copied * sizeof(wchar_t));
    m_TextLen = static_cast<u16>(offset + copied);
    m_pTextBuf[m_TextLen] = 0;
    m_IsPTDirty |= 1;
    return copied;
}

} }
