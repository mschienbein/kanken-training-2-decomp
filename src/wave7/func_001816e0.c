#include "game/screen.h"

void func_001816e0(struct BoundDisplayObject *object, float x, float y)
{
    struct DisplayPane *pane = object->bindings[0]->pane;
    pane->scale[0] = x;
    pane->scale[1] = y;
    pane->flags &= 0xcf;
}
