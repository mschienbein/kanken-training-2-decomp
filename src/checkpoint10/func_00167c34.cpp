#include <preinclude.h>
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Color.h"

#include "game/screen.h"

void func_00167c34(nw::lyt::Pane *pane, unsigned char red,
    unsigned char green, unsigned char blue, unsigned char alpha)
{
    if (pane != 0) {
        nw::ut::Color8 color(red, green, blue, alpha);
        pane->SetVtxColor(0, color);
        pane->SetVtxColor(1, color);
        pane->SetVtxColor(2, color);
        pane->SetVtxColor(3, color);
    }
}
