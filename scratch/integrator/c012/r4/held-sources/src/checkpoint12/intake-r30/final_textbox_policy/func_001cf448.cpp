/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/lyt/lyt_TextBox.cpp.
 * Target-specific dirty storage uses the shared canonical native header.
 */
#include "game/textbox.h"
#include <cstring>
#include <nw/ut/ut_Inlines.h>

static inline unsigned short string_capacity(const nw::lyt::TextBox* object)
{
    return object->m_TextBufBytes ? static_cast<unsigned short>(object->m_TextBufBytes / 2 - 1) : 0;
}

static void publish_end(nw::lyt::TextBox* object, unsigned end)
{
    object->m_TextLen = static_cast<unsigned short>(end);
    object->m_pTextBuf[object->m_TextLen] = 0;
    object->m_IsPTDirty |= 1;
}

unsigned short func_001cf448(nw::lyt::TextBox* object, const wchar_t* text,
                             unsigned short offset, unsigned length)
{
    if (!object->m_pFont)
        return 0;
    if (!object->m_pTextBuf)
        return 0;
    const unsigned short capacity = string_capacity(object);
    if (offset >= capacity)
        return 0;
    if (object->m_TextLen < offset)
        return 0;
    if (length && (!text || !*text))
        return 0;
    unsigned copied = nw::ut::Min(length, unsigned(capacity - offset));
    /* ORIGINAL BUG: zero copy can pass null to memcpy. */
    std::memcpy(object->m_pTextBuf + offset, text, copied * sizeof(wchar_t));
    publish_end(object, offset + copied);
    return copied;
}
