#include "game/exp_heap.h"
#include "game/runtime.h"
void func_0019dd00(struct HeapArena *arena, void *memory, unsigned size,
                   unsigned attributes)
{
    if (!func_001219f4(&arena->heap, memory, size, attributes))
        func_0019f568();
    arena->allocations = 0;
}
