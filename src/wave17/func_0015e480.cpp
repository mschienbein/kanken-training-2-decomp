#include <preinclude.h>
#include "game/exp_heap.h"
#include "nw/lyt/lyt_TextBox.h"
#include "game/screen.h"

extern "C" void func_0015e480(struct ResourceEntry *link, float x, float y)
{
    nw::lyt::Pane *pane = link->object;
    pane->m_Scale.x = x;
    pane->m_Scale.y = y;
    pane->m_Flag &= 0xcf;
}
