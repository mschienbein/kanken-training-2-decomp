#include "game/screen.h"

struct W11jFlagBinding {
    struct DisplayObject *object;
    unsigned flags;
};

struct W11jBindingOwner {
    unsigned char reserved00[0x30];
    struct W11jFlagBinding *binding;
};

void func_0017939c(struct W11jBindingOwner *owner, int enabled)
{
    struct W11jFlagBinding *binding = owner->binding;
    if (enabled) {
        if (binding != 0 && binding->object != 0)
            binding->flags &= ~1u;
    } else {
        if (binding != 0 && binding->object != 0)
            binding->flags |= 1;
    }
}
