#include "game/object_pool.h"
#include "game/screen.h"


void func_001c1bf0(struct DisplayHandle **handles, int index, int value)
{
    struct DisplayHandle *handle = handles[index];
    if (handle)
        func_0017a968(handle->object ? &handle->object->base : 0,
                     bss_00290e2c[index], value, 1);
}
