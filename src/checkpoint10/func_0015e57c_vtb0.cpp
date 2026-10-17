#include <preinclude.h>
#include <nw/lyt/lyt_TextBox.h>
using nw::ut::Color8;
struct DisplaySurface {
    unsigned char reserved00[8];
    nw::lyt::TextBox *pane;
};
extern "C" void func_0015e57c(DisplaySurface *surface, unsigned char red, unsigned char green,
                              unsigned char blue, unsigned char alpha)
{
    surface->pane->SetTextColor(nw::lyt::TEXTCOLOR_TOP, Color8(red, green, blue, alpha));
}
