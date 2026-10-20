#define NW_RELEASE
#include "game/screen.h"
#include "nw/ut/ut_Color.h"


static __inline void set_top(struct DisplaySurface *surface, unsigned char red,
                             unsigned char green, unsigned char blue, unsigned char alpha)
{
    surface->text->SetTextColor(0, nw::ut::Color8(red, green, blue, alpha));
}

static __inline void set_bottom(struct DisplaySurface *surface, unsigned char red,
                                unsigned char green, unsigned char blue, unsigned char alpha)
{
    surface->text->SetTextColor(1, nw::ut::Color8(red, green, blue, alpha));
}


void func_0015e3f4(DisplaySurface *surface, unsigned char red,
                  unsigned char green, unsigned char blue, unsigned char alpha)
{
    set_top(surface, red, green, blue, alpha);
    set_bottom(surface, red, green, blue, alpha);
}
