#include "game/object_pool.h"
#include "game/sized_buffer.h"
/* The initializer consumes size and returns its incoming buffer pointer.
 * [INFERENCE] The original constructor spelling and object name are unrecovered. */
SizedBuffer *func_0010c4f4(SizedBuffer *buffer, unsigned size)
{
    if (buffer)
        return func_0011481c(buffer, size);
    return buffer;
}
