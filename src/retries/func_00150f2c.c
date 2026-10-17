#include "game/service_state.h"
void func_00150f2c(struct S150f2c *object, unsigned mask, float value)
{
    int i = 0;
    while (i < object->count && mask != 0) {
        if (mask & 1)
            object->records[i].value = value;
        ++i;
        mask /= 2u;
    }
}
