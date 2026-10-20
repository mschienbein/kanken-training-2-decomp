#include "game/w11_control.h"
void func_0016f9a8(struct VisualLabelSet72 *owner, int index, float x, float y)
{
    struct VisualLabelGroup10 *node = owner->groups[index];
    if (node != 0) {
        node->scale.x = x;
        node->scale.y = y;
    }
}
