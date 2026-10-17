#include "game/font_resource.h"
int func_00149494(struct VramAllocator *allocator, unsigned size, unsigned alignment)
{
    int slot;
    return func_0018d280(allocator, size, alignment, 0, &slot, 0);
}
