#include "game/layout_lifecycle.h"



void func_0016b260(void *allocation)
{
    Heap *allocator = data_00242f84.secondary;
    allocator->release(allocation);
}
