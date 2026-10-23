#define NW_RELEASE
#include <nw/font/font_CharStrmReader.h>

namespace nw { namespace font {
u16 CharStrmReader::ReadNextCharCP1252()
{
    const u8 *current = static_cast<const u8 *>(m_CharStrm);
    u16 value = *current++;
    m_CharStrm = current;
    return value;
}
} }
