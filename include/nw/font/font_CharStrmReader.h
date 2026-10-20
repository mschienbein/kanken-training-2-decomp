/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/font/font_CharStrmReader.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public layouts; normalize assertion includes and remove unused SDK/module and graphics-type dependencies.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <nw/types.h>
#include <nw/assert.h>

namespace nw {
namespace font {

class CharStrmReader
{
    friend class Font;
public:
    typedef u16 (CharStrmReader::*ReadNextCharFunc)();
    CharStrmReader(const CharStrmReader& rhs):
        m_CharStrm(rhs.m_CharStrm),
        m_ReadFunc(rhs.m_ReadFunc) {}

    ~CharStrmReader() {}

    void Set(const char* stream)
    {
        this->m_CharStrm = stream;
    }

    void Set(const wchar_t* stream)
    {
        this->m_CharStrm = stream;
    }
    const void* GetCurrentPos() const { return m_CharStrm; }
    u16 Next() { return (this->*m_ReadFunc)(); }
private:

    const void* m_CharStrm;
    const ReadNextCharFunc m_ReadFunc;

    explicit CharStrmReader(ReadNextCharFunc func):
        m_CharStrm(NULL),
        m_ReadFunc(func) {}

    template<typename CharType>
    CharType GetChar(int offset = 0) const
    {
        const CharType* charStrm = reinterpret_cast<const CharType*>(m_CharStrm);
        return *(charStrm + offset);
    }

    template<typename CharType>
    void StepStrm(int step = 1)
    {
        const CharType*& charStrm = reinterpret_cast<const CharType*&>(m_CharStrm);
        charStrm += step;
    }

    u16 ReadNextCharUTF8();
    u16 ReadNextCharUTF16();
    u16 ReadNextCharCP1252();
    u16 ReadNextCharSJIS();

    /* [INFERENCE] Distinct emitted copies selected by data_0021c64c.
     * These are genuine members of this owner, not free-function facades. */
    u16 ReadNextCharUTF8Copy();
    u16 ReadNextCharSJISCopy();
};

}
}

/* The original descriptor uses this exact native member-pointer protocol. */
extern "C" const nw::font::CharStrmReader::ReadNextCharFunc data_0021c64c[4];

