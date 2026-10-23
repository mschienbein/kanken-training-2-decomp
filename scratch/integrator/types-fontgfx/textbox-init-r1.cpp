#include <preinclude.h>
#include <nw/lyt/lyt_TextBox.h>

namespace nw { namespace lyt {
void TextBox::Init(u16 length, unsigned format)
{
    m_pTextBuf = 0;
    m_TextBufBytes = 0;
    m_TextLen = 0;
    m_pFont = 0;
    m_FontSize.width = 0.0f;
    m_FontSize.height = 0.0f;
    m_TextPosition = u8(m_TextPosition / 3 * 3 + 1);
    m_TextPosition = u8(m_TextPosition % 3 + 3);
    m_LineSpace = 0.0f;
    m_CharSpace = 0.0f;
    m_pTagProcessor = 0;
    m_pDispStringBuf = 0;
    m_TextAlignment = 0;
    m_IsPTDirty = 0;
    AllocStringBuffer(length, format);
}
} }
