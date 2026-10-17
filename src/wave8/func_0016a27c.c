#include "game/scene_lifecycle.h"

void func_0016a27c(struct ManagedResource *element, int enabled)
{
    if (element->enabled != enabled) {
        unsigned flags = element->flags;
        element->enabled = enabled;
        enabled = flags | 2;
        element->flags = enabled;
    }
}
