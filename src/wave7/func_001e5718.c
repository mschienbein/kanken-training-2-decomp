#include "game/screen.h"

struct BoundDisplayObject *func_001e5718(struct BoundDisplayObject *object, float width, float height)
{
    if (object != 0)
        func_00181734(object, (float)(int)width, height);
    return object;
}
