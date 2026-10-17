#include "game/service_state.h"
void func_00150c58(struct S150c58 *object, unsigned mask, float value)
{
    int i = 0;
    while (i < object->count && mask != 0) {
        if (mask & 1)
            object->records[i].value = value;
        ++i;
        mask /= 2u;
    }
}
