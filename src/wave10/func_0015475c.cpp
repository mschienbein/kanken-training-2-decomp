#include "game/prefetch_stream.h"

extern "C" void func_0015475c(struct StreamChannel *stream, struct RecordState *block, int update)
{
    if (stream->channel != 0)
        func_0017da08(stream->channel, 0, block, update);
}
