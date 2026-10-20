#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

namespace nw { namespace font {
u16 CharStrmReader::ReadNextCharUTF16()
{
    const u16 *current = static_cast<const u16 *>(m_CharStrm);
    u16 value = *current++;
    m_CharStrm = current;
    return value;
}
} }
