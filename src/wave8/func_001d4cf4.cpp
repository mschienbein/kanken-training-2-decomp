#include "game/scene_lifecycle.h"

void func_001d4cf4(struct SceneChild *parent, bool enabled)
{
    struct SceneChild *child = parent->children;
    while (child) {
        if (child->active && (child->enabled != 0) != enabled) {
            child->enabled = enabled;
            func_0016a27c(child->element, enabled);
        }
        child = child->next;
    }
}
