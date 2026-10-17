#include "game/w11_control.h"
void func_001eaa7c(struct W11jVisualOwner *owner, int index,
                   unsigned char red, unsigned char green,
                   unsigned char blue, unsigned char alpha)
{
    struct W11jVisualNode *node = owner->nodes[index];
    if (node != 0) {
        node->color[0] = red;
        node->color[1] = green;
        node->color[2] = blue;
        node->color[3] = alpha;
    }
}
