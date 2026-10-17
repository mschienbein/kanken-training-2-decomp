#include "game/object_pool.h"
unsigned int func_001932d8(const struct SizedBufferHandle *handle)
{
    return handle->buffer ? handle->buffer->size : 0;
}
