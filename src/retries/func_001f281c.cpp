#include "game/memory.h"
#include "game/runtime.h"
#include <stdint.h>


bool func_001f281c(struct PackedRecordSource *source, void *output, unsigned int index)
{
    unsigned char storage[0x11f];
    source->stream->seek(source->base + source->stride * index, 0);
    unsigned char *buffer = (unsigned char *)(((uintptr_t)storage + 31u) & ~31u);
    if (source->stream->read(buffer, 0x100) != 0x100)
        return false;
    func_00192720(output, buffer, 0x100);
    return true;
}
