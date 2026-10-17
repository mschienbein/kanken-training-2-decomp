#define NW_RELEASE
#include <nw/font/font_Font.h>

/* INTEGRATOR: move to include/game/render_state.h; matches src/wave2/func_0018f0ac.c. */
extern "C" {
unsigned func_0018f0ac(const void *address);
}

namespace nw { namespace font { namespace internal {
void TextureObject::Set(u32 name, const Font *font, const void *image,
                        TexFmt format, u16 width, u16 height)
{
    m_Name = name;
    m_pFont = font;
    m_pImage = image != 0 ? func_0018f0ac(image) >> 3 : 0;
    m_Format = static_cast<u8>(format);
    m_Size.HW.width = width;
    m_Size.HW.height = height;
}
} } }
