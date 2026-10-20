#include <preinclude.h>
#include "game/exp_heap.h"
#include "nw/lyt/lyt_TextBox.h"
#include "game/screen.h"

extern "C" void func_0015e954(struct ResourceEntry *link, int active)
{
    link->object->m_Flag = active | (link->object->m_Flag & 0xfe);
}
