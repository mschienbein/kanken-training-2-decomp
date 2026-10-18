#include "game/scene_lifecycle.h"

extern void func_001835b8(struct ManagedResource *resource);

void func_00183758(struct SceneChild *child)
{
    if (child->element == 0)
        return;
    func_001835b8(child->element);
    func_00183550(child->element);
    child->element = 0;
    child->enabled = 0;
    child->active = 0;
    if (child->callback != 0)
        child->callback(child, 0, child->context);
    if (child->state != 0) {
        child->state = 0;
        func_00182994(func_00185cc0(), child);
    }
}
