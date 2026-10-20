#include "game/heap_interface.h"
#include "game/resource_instance.h"

/* Heap matches src/pilot/game_heap.cpp exactly. */

void *func_00116cb4(unsigned size, unsigned alignment)
{
    return data_00242f84.secondary->allocate(size, alignment);
}
