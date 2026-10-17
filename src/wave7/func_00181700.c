#include "game/screen.h"

/* [INFERENCE] The isolated r1-zero writes do not establish an input: known
 * matching callers prepare only object plus s0-s2. See the r2 review rejection. */
void func_00181700(struct BoundDisplayObject *object, float x, float y, float z)
{
    struct DisplayPane *pane = object->bindings[0]->pane;
    pane->position[0] = x;
    pane->position[1] = y;
    pane->position[2] = z;
    pane->flags &= 0xcf;
    object->base.position[2] = z;
}
