#include "game/layout_lifecycle.h"



void func_0016b260(void *allocation)
{
    W17Allocator *allocator = data_00242f84.temporary;
    allocator->vtable->release(allocator, allocation);
}
