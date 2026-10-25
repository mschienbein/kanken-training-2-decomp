#include "game/sound_pool.h"

namespace {
struct AlignedChannelSpan {
    unsigned bytes;
    explicit AlignedChannelSpan(unsigned quotient) : bytes(quotient & ~31u) {}
};
}
extern "C" void func_00147390(ChannelBuffer *buffer, void *storage,
                              unsigned size, unsigned channels)
{
    if (channels == 0)
        return;
    buffer->size = size;
    buffer->channel_count = channels;
    buffer->base = storage;
    AlignedChannelSpan span(size / channels);
    buffer->flags = 0;
    buffer->channel_size = span.bytes;
    buffer->allocated = 0;
}

