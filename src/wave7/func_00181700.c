#include "game/screen.h"

void func_00181700(struct BoundDisplayObject *object, float x, float y, float z)
{
    struct DisplayPane *pane = object->bindings[0]->pane;
    pane->position[0] = x;
    pane->position[1] = y;
    pane->position[2] = z;
    pane->flags &= 0xcf;
    object->base.position[2] = z;
}
