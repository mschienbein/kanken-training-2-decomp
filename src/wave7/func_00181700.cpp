#include <preinclude.h>
#include "nw/lyt/lyt_Picture.h"
#include "game/screen.h"

/* [INFERENCE] The isolated r1-zero writes do not establish an input: known
 * matching callers prepare only object plus s0-s2. See the r2 review rejection. */
extern "C" void func_00181700(struct DisplayObject *object, float x, float y, float z)
{
    nw::lyt::Pane *pane = display_pane_attachment(object, 0)->picture;
    pane->m_Translate.x = x;
    pane->m_Translate.y = y;
    pane->m_Translate.z = z;
    pane->m_Flag &= 0xcf;
    object->position[2] = z;
}
