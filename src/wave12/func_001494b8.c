#include "game/font_resource.h"
int func_001494b8(struct VramAllocator *allocator, unsigned size, unsigned alignment)
{
    int slot;
    return func_0018d280(allocator, size, alignment, 1, &slot, 0);
}
