#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"


extern "C" void func_001826fc(struct DisplayObject *object, int active)
{
    nw::lyt::Pane *pane = display_pane_attachment(object, 0)->picture;
    pane->m_Flag = (pane->m_Flag & 0xfe) | active;
}
