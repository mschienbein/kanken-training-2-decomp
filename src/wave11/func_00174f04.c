#include "game/command_state.h"
struct VectorParameter *func_00174f04(struct VectorParameter *parameter,
                                    int extra, int first, int second,
                                    float x, float y, float z)
{
    parameter->extra = extra;
    parameter->kind = 0;
    parameter->first = first;
    parameter->second = second;
    parameter->x = x;
    parameter->y = y;
    parameter->z = z;
    return parameter;
}
