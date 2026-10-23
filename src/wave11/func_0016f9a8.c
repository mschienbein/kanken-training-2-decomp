#include "game/w11_control.h"
void func_0016f9a8(struct W11jVisualOwner *owner, int index, float x, float y)
{
    struct W11jVisualNode *node = owner->nodes[index];
    if (node != 0) {
        node->scale_x = x;
        node->scale_y = y;
    }
}
