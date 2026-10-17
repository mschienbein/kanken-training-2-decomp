#include "game/scene_lifecycle.h"

struct ManagedResource *func_00138cb0(struct ManagedResource *resource)
{
    int i;
    for (i = 0; i < 2; ++i) {
        if (resource->handles[i])
            func_001829d0(resource->handles[i]);
    }
    return resource;
}
