#include "game/scene_lifecycle.h"

void func_0018290c(struct ManagedResource *resource, unsigned event,
                   struct SceneChild *child)
{
    unsigned notification = 2;
    switch (event) {
    case 0:
        func_00183550(resource);
        break;
    case 1:
        notification = 3;
        func_00183550(resource);
        break;
    case 2:
    case 3:
        notification = 1;
        break;
    }
    if (child->callback)
        child->callback(child, notification, child->context);
    child->element = 0;
    child->enabled = 0;
    child->active = 0;
    child->state = 0;
    func_00182994(func_00185cc0(), child);
}
