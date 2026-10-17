#include "game/result.h"

struct SceneResource;
class SceneAllocator;

/* INTEGRATOR: move to include/game/scene_resource.h */
extern "C" {
Result func_001c65ac(SceneResource *resource, SceneAllocator *allocator);
Result func_001c5f70(SceneResource *resource, SceneAllocator *allocator);
Result func_001c8e10(SceneResource *resource, SceneAllocator *allocator);
Result func_001c5e4c(SceneResource *resource, SceneAllocator *allocator);
}

Result func_001c5e4c(SceneResource *resource, SceneAllocator *allocator)
{
    Result result;
    result = Result(func_001c65ac(resource, allocator).value | result.value);
    if (result.value & 0x80000000u)
        return result;
    result = Result(func_001c5f70(resource, allocator).value | result.value);
    if (result.value & 0x80000000u)
        return result;
    result = Result(func_001c8e10(resource, allocator).value | result.value);
    return result;
}
