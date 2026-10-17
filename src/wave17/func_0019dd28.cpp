#include "game/exp_heap.h"



extern "C" void func_0019dd28(HeapArena *arena)
{
    if (arena->heap.base.signature != 0) {
        func_00121bc4(&arena->heap);
        arena->heap.base.signature = 0;
    }
}
