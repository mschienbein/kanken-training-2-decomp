#include "game/resource_tables.h"
extern int func_0017fe98(int slot, const void *data, int format, int options);

int func_0017fe60(int slot, uintptr_t resource, int index, int format, int options)
{
    return func_0017fe98(slot, func_001971a0(resource, index), format, options);
}
