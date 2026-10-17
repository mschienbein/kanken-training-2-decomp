#include "game/service_state.h"
void func_00150f6c(struct S150f6c *object, unsigned mask, int enabled)
{
    int i = 0;
    unsigned char value = (unsigned char)enabled;
    while (i < object->count && mask != 0) {
        if (mask & 1)
            object->records[i].enabled = value;
        ++i;
        mask /= 2u;
    }
}
