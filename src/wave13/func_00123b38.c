#include "game/input_device.h"
/* [INFERENCE] Layout and constructor result are evidenced; field roles are unknown. */

struct InputBufferState *func_00123b38(struct InputBufferState *buffer)
{
    buffer->storage = 0;
    buffer->count = 0;
    buffer->capacity = 0;
    buffer->enabled = 0;
    return buffer;
}
