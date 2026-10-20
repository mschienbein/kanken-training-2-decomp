#include <preinclude.h>
#include "game/exp_heap.h"
#include "nw/lyt/lyt_TextBox.h"
#include "game/screen.h"

extern "C" void func_0015e698(struct ResourceEntry *link, float x, float y, float z)
{
    nw::lyt::Pane *pane = link->object;
    pane->m_Translate.x = x;
    pane->m_Translate.y = y;
    pane->m_Translate.z = z;
    pane->m_Flag &= 0xcf;
}
