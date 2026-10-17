#include <string.h>
#include "game/runtime.h"
void func_0018f0b0(struct ByteWriter *writer, const void *source, unsigned size)
{
    memcpy(writer->data + writer->position, source, size);
    writer->position += size;
}
