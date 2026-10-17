#include "game/screen.h"


void func_001826fc(struct DisplayObject *object, int active)
{
    struct BoundDisplayObject *display = (struct BoundDisplayObject *)object;
    struct DisplayPane *pane = display->bindings[0]->pane;
    pane->flags = (pane->flags & 0xfe) | active;
}
