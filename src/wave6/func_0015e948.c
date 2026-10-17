#include "game/screen.h"


void func_0015e948(struct DisplayObject *object, int active)
{
    struct BoundDisplayObject *display = (struct BoundDisplayObject *)object;
    func_0015e954(display->bindings[0]->link, active);
}
