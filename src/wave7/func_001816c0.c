#include "game/screen.h"

void func_001816c0(struct BoundDisplayObject *object, float x, float y, float z)
{
    struct DisplayPane *pane = object->bindings[0]->pane;
    pane->rotation[0] = x;
    pane->rotation[1] = y;
    pane->rotation[2] = z;
    pane->flags &= 0xcf;
}
