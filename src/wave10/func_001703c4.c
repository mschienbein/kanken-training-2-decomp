#include "game/input.h"

int func_001703c4(const struct InputRectangle *rectangle, float x, float y)
{
    if (rectangle->x > x || rectangle->x + rectangle->width < x ||
        rectangle->y > y)
        return 0;
    if (rectangle->y + rectangle->height < y)
        return 0;
    return 1;
}
