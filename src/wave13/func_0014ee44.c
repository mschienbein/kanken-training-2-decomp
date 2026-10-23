#include "game/input_device.h"
/* [INFERENCE] Array-constructor callback; field roles are not yet recovered. */

void *func_0014ee44(void *storage)
{
    struct DisplayBufferState *buffer = storage;
    buffer->storage = 0;
    buffer->count = 0;
    buffer->capacity = 0;
    buffer->enabled = 0;
    return buffer;
}
