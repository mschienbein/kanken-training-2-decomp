#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"

extern "C" void func_00181734(struct DisplayObject *object, float width, float height)
{
    nw::lyt::Pane *pane = display_pane_attachment(object, 0)->picture;
    pane->m_Size.width = width;
    pane->m_Size.height = height;
}
