#include "game/exp_heap.h"



extern "C" void func_0019dd28(HeapArena *arena)
{
    if (arena->base.heap.base.signature != 0) {
        func_00121bc4(&arena->base.heap);
        arena->base.heap.base.signature = 0;
    }
}
