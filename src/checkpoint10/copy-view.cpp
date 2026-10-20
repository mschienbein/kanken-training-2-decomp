#define NW_RELEASE
#include <nw/types.h>

/* [INFERENCE] This image holds a second copy of nw::font::CharStrmReader's UTF-8 reader
 * (entry reached only through a pointer-to-member table). The copy cannot take the
 * canonical mangled name, which is bound to 0x001c5ad4, so it is authored as a free function
 * over an equivalent single-pointer stream view. */
struct CharStreamView {
    const void *m_CharStrm;

    template<typename CharType>
    CharType GetChar(int offset = 0) const
    {
        const CharType *charStrm = reinterpret_cast<const CharType *>(m_CharStrm);
        return *(charStrm + offset);
    }

    template<typename CharType>
    void StepStrm(int step = 1)
    {
        const CharType *&charStrm = reinterpret_cast<const CharType *&>(m_CharStrm);
        charStrm += step;
    }
};

extern "C" u16 func_001dafe8(CharStreamView *reader)
{
    u16 code;

    /* ORIGINAL BUG: continuation bytes, truncation and four-byte UTF-8 are not validated. */
    if (!(reader->GetChar<u8>(0) & 0x80)) {
        code = reader->GetChar<u8>(0);
        reader->StepStrm<u8>(1);
    } else if ((reader->GetChar<u8>(0) & 0xE0) == 0xC0) {
        code = ((reader->GetChar<u8>(0) & 0x1F) << 6) | (reader->GetChar<u8>(1) & 0x3F);
        reader->StepStrm<u8>(2);
    } else {
        code = ((reader->GetChar<u8>(0) & 0x1F) << 12) | ((reader->GetChar<u8>(1) & 0x3F) << 6) |
               (reader->GetChar<u8>(2) & 0x3F);
        reader->StepStrm<u8>(3);
    }

    return code;
}
