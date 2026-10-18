#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

namespace nw {
namespace font {

u16 CharStrmReader::ReadNextCharUTF8()
{
    u16 code;

    /* ORIGINAL BUG: continuation bytes, truncation and four-byte UTF-8 are not validated. */
    if (!(GetChar<u8>(0) & 0x80)) {
        code = GetChar<u8>(0);
        StepStrm<u8>(1);
    } else if ((GetChar<u8>(0) & 0xE0) == 0xC0) {
        code = ((GetChar<u8>(0) & 0x1F) << 6) | (GetChar<u8>(1) & 0x3F);
        StepStrm<u8>(2);
    } else {
        code = ((GetChar<u8>(0) & 0x1F) << 12) | ((GetChar<u8>(1) & 0x3F) << 6) |
               (GetChar<u8>(2) & 0x3F);
        StepStrm<u8>(3);
    }

    return code;
}

} // namespace font
} // namespace nw
