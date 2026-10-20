#include "game/resource_stream.h"

extern "C" int func_001c4494(ResourceFile *file, void *buffer, unsigned size)
{
    unsigned count;
    Result result(func_0019d5e4(&file->stream, &count, buffer, size));
    unsigned pc = __current_pc();
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
    return count;
}
