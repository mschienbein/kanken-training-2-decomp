#include "game/material.h"
struct MaterialPair *func_0014d254(struct Material *material)
{
    unsigned int capacity = material->capacity;
    unsigned int offset = (capacity & 3u) * 32u +
        ((capacity >> 2) & 3u) * 20u + ((capacity >> 4) & 3u) * 4u;
    return (struct MaterialPair *)(material->parameters + offset);
}
