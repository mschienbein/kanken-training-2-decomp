#include "game/input.h"

float func_00167000(const struct InputRectangle *rectangle, float value)
{
    if (rectangle->y > value)
        value = rectangle->y;
    else if (rectangle->y + rectangle->height < value)
        value = rectangle->y + rectangle->height;
    return value;
}
