/* The pinned native headers support their upstream release assertion mode. */
#define NW_RELEASE
#include <nw/font/font_Font.h>

typedef unsigned short (nw::font::CharStrmReader::*ReaderMethod)();
extern "C" const ReaderMethod data_0021c64c[4];

namespace nw {
namespace font {
const CharStrmReader Font::GetCharStrmReader(wchar_t dummy) const
{
    CharStrmReader::ReadNextCharFunc next = 0;
    if (GetCharacterCode() == CHARACTER_CODE_UNICODE)
        next = data_0021c64c[3];
    return CharStrmReader(next);
}
}
}
