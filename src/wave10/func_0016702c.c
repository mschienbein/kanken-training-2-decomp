#include "game/input.h"

float func_0016702c(const struct InputRectangle *rectangle, float value)
{
    if (rectangle->x > value)
        value = rectangle->x;
    else if (rectangle->x + rectangle->width < value)
        value = rectangle->x + rectangle->width;
    return value;
}
