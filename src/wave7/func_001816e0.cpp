#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"

extern "C" void func_001816e0(struct DisplayObject *object, float x, float y)
{
    nw::lyt::Pane *pane = display_pane_attachment(object, 0)->picture;
    pane->m_Scale.x = x;
    pane->m_Scale.y = y;
    pane->m_Flag &= 0xcf;
}
