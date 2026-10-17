#include "game/command_state.h"
struct ParameterRecord *func_00175114(struct ParameterRecord *parameter,
                                    int kind, int first, int second, int third,
                                    float x, float y, float z)
{
    parameter->kind = kind;
    parameter->first = first;
    parameter->second = second;
    parameter->third = third;
    parameter->x = x;
    parameter->y = y;
    parameter->z = z;
    return parameter;
}
