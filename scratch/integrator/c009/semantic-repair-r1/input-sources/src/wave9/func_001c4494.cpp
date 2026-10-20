#include "game/resource_stream.h"

extern "C" int func_001c4494(ResourceFile *file, void *buffer, unsigned size)
{
    unsigned count;
    int result = func_0019d5e4(&file->stream, &count, buffer, size);
    if (result < 0)
        func_0019d6c4((unsigned)result, __current_pc());
    return count;
}
