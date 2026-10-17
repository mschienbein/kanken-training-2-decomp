#include "game/screen.h"

void func_00181734(struct BoundDisplayObject *object, float width, float height)
{
    struct DisplayPane *pane = object->bindings[0]->pane;
    pane->size[0] = width;
    pane->size[1] = height;
}
