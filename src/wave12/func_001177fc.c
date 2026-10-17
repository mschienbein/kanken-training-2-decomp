#include "game/font_resource.h"
#include "game/runtime.h"


void *func_001177fc(struct VramAllocator *allocator, unsigned size, unsigned alignment)
{
    int slot;
    if (func_0018d280(allocator, size, alignment, 1, &slot, 1) && slot > 0) {
        struct VramNode *nodes = allocator->nodes[1];
        return (nodes + slot)->address;
    }
    func_0019f568();
    return 0;
}
