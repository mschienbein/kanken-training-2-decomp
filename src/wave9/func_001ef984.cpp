#include "game/resource_stream.h"

static inline void check_result(unsigned pc, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

extern "C" unsigned long long func_001ef984(ResourceFile *file)
{
    ResourceSize size;
    Result result = func_0019d66c(&file->stream, &size);
    check_result(__current_pc(), result);
    return size.get();
}
