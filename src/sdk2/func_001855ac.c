#include <string.h>
#include "game/runtime.h"
void func_001855ac(struct ByteWriter *writer, const void *source, unsigned size)
{
    struct ByteWriter snapshot = *writer;
    memcpy(snapshot.data + snapshot.position, source, size);
    writer->position += size;
}
