#include <preinclude.h>
#include "game/exp_heap.h"
#include "game/screen.h"
#include "nw/lyt/lyt_TextBox.h"

/* D08: 0015d798 constructs the TextBox held by this ResourceEntry. */
extern "C" void func_0015e5d4(ResourceEntry *entry, float x, float y, float z)
{
    nw::lyt::Pane *pane = entry->object;
    pane->m_Rotate.x = x;
    pane->m_Rotate.y = y;
    pane->m_Rotate.z = z;
    pane->m_Flag &= 0xcf;
}
