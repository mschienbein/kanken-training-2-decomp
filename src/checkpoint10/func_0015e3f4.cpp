#define NW_RELEASE
#include "game/screen.h"
#include "nw/ut/ut_Color.h"

struct DisplayPaneColors {
    unsigned char reserved00[0xd8];
    nw::ut::Color8 colors[2];
    void SetTextColor(unsigned index, nw::ut::Color8 value)
    {
        colors[index] = value;
    }
};

struct DisplaySurface {
    unsigned char reserved00[8];
    DisplayPaneColors *pane;
};

static __inline void set_top(struct DisplaySurface *surface, unsigned char red,
                             unsigned char green, unsigned char blue, unsigned char alpha)
{
    surface->pane->SetTextColor(0, nw::ut::Color8(red, green, blue, alpha));
}

static __inline void set_bottom(struct DisplaySurface *surface, unsigned char red,
                                unsigned char green, unsigned char blue, unsigned char alpha)
{
    surface->pane->SetTextColor(1, nw::ut::Color8(red, green, blue, alpha));
}

/* INTEGRATOR: move to include/game/screen.h. */
extern "C" {
void func_0015e3f4(DisplaySurface *surface, unsigned char red,
                  unsigned char green, unsigned char blue, unsigned char alpha);
}

void func_0015e3f4(DisplaySurface *surface, unsigned char red,
                  unsigned char green, unsigned char blue, unsigned char alpha)
{
    set_top(surface, red, green, blue, alpha);
    set_bottom(surface, red, green, blue, alpha);
}
