#include "game/resource_stream.h"

static inline int is_success(Result result)
{
    return (int)result.value >= 0;
}

static inline void check_result(unsigned pc, Result result)
{
    switch (result.failed()) {
    case 0: break;
    default: func_0019d6c4(result.value, pc); break;
    }
}

static inline void seek_checked(ResourceFile *file, long long offset, int origin)
{
    Result result = func_00180944(&file->stream, offset, origin);
    check_result(__current_pc(), result);
}

extern "C" int func_00146dfc(ResourceFile *file, int offset, int origin)
{
    int seek_origin;
    /* ORIGINAL BUG: an invalid origin leaves the translated origin undefined. */
    switch (origin) {
    case 0: seek_origin = 0; break;
    case 1: seek_origin = 1; break;
    case 2: seek_origin = 2; break;
    }
    if (file->recoverable) {
        file->result = func_00180944(&file->stream, offset, seek_origin);
        return is_success(file->result);
    }
    seek_checked(file, offset, seek_origin);
    return 1;
}
