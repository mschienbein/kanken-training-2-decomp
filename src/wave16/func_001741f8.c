#include "game/w11_control.h"

void func_001741f8(struct W11jVisualOwner *set, int index, float x, float y)
{
    struct W11jVisualNode *object = set->nodes[index];
    if (object != 0) {
        object->position[0] = x;
        object->position[1] = y;
    }
}
