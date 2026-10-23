struct MemoryWriteStream {
    unsigned char reserved00[0x14];
    const void *data;
    unsigned size;
};

#include <string.h>

unsigned char *func_001dac84(const struct MemoryWriteStream *stream,
                             unsigned char *destination, unsigned flags)
{
    if (!(flags & 1)) {
        unsigned char *start = destination;
        memcpy(start, stream->data, stream->size);
        destination = start + (stream->size & ~3u);
    }
    return destination;
}
