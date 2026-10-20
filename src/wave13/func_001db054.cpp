#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

namespace nw { namespace font {
u16 CharStrmReader::ReadNextCharUTF16()
{
    const wchar_t *current = static_cast<const wchar_t *>(m_CharStrm);
    u16 value = *current++;
    m_CharStrm = current;
    return value;
}
} }
