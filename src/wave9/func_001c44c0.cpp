#include "game/resource_stream.h"

static inline void check_result(unsigned pc, Result result)
{
    /* Qualification: preserve the diagnostic PC capture before the result test. */
    __schedule_barrier();
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

extern "C" void func_001c44c0(ResourceFile *file, long long offset, int origin)
{
    Result result = func_00180944(&file->stream, offset, origin);
    check_result(__current_pc(), result);
}
