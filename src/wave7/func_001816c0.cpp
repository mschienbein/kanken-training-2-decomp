#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"

extern "C" void func_001816c0(struct DisplayObject *object, float x, float y, float z)
{
    nw::lyt::Pane *pane = display_pane_attachment(object, 0)->picture;
    pane->m_Rotate.x = x;
    pane->m_Rotate.y = y;
    pane->m_Rotate.z = z;
    pane->m_Flag &= 0xcf;
}
