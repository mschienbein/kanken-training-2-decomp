#include <preinclude.h>
#include <nw/lyt/lyt_TextBox.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Layout.h>
#include "game/layout_lifecycle.h"

namespace nw { namespace lyt {
void TextBox::FreeStringBuffer()
{
    if (m_pTextBuf) {
        Layout::FreeMemory(m_pTextBuf);
        m_pTextBuf = 0;
        m_TextBufBytes = 0;
        m_TextLen = 0;
    }
    if (m_pDispStringBuf) {
        if (m_pDispStringBuf->allocationFlags & 8)
            func_0016b260(m_pDispStringBuf);
        else
            Layout::FreeMemory(m_pDispStringBuf);
        m_pDispStringBuf = 0;
    }
}
TextBox::~TextBox()
{
    Material *material = m_pMaterial;
    if (material && !material->IsUserAllocated()) {
        material->~Material();
        Layout::FreeMemory(material);
        m_pMaterial = 0;
    }
    FreeStringBuffer();
}
} }
