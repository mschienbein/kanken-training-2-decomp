#include "game/w11_control.h"

void func_001741f8(struct VisualLabelSet72 *set, int index, float x, float y)
{
    struct VisualLabelGroup10 *object = set->groups[index];
    if (object != 0) {
        object->position.x = x;
        object->position.y = y;
    }
}
