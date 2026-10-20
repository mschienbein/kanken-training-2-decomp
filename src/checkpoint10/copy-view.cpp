#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

/* [INFERENCE] The original secondary descriptor selects these two distinct
 * emitted members. Canonical UTF16/CP1252 providers retain their own owners;
 * Copy suffixes distinguish only these UTF8/SJIS bodies from the primary ones. */
u16 nw::font::CharStrmReader::ReadNextCharUTF8Copy()
{
    u16 code;
    /* ORIGINAL BUG: continuation bytes, truncation and four-byte UTF-8 are not validated. */
    if (!(GetChar<u8>(0) & 0x80)) {
        code = GetChar<u8>(0);
        StepStrm<u8>(1);
    } else if ((GetChar<u8>(0) & 0xe0) == 0xc0) {
        code = ((GetChar<u8>(0) & 0x1f) << 6) | (GetChar<u8>(1) & 0x3f);
        StepStrm<u8>(2);
    } else {
        code = ((GetChar<u8>(0) & 0x1f) << 12) | ((GetChar<u8>(1) & 0x3f) << 6) |
               (GetChar<u8>(2) & 0x3f);
        StepStrm<u8>(3);
    }
    return code;
}

u16 nw::font::CharStrmReader::ReadNextCharSJISCopy()
{
    u16 character = GetChar<u8>();
    if (static_cast<unsigned int>(character - 0x81) < 0x1f || character >= 0xe0) {
        character = (character << 8) | GetChar<u8>(1);
        StepStrm<u8>(2);
    } else {
        StepStrm<u8>();
    }
    return character;
}

