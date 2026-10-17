#include "game/resource_stream.h"
static inline void check_result(unsigned pc, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

extern "C" int func_001c4494(ResourceFile *file, void *buffer, unsigned size)
{
    unsigned count;
    Result result = func_0019d5e4(&file->stream, &count, buffer, size);
    check_result(__current_pc(), result);
    return count;
}
