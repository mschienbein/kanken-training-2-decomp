#include "game/display_animation.h"
#include "game/result.h"


extern "C" Result func_0012e0c0(PositionableStream *stream, long long offset)
{
    if (stream != 0)
        return stream->vtable->seek(stream, offset);
    return Result(0xe0e046bcu);
}
