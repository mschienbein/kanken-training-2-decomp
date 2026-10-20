#include "game/file_stream.h"



extern "C" Result func_0019d5dc(FileStreamView *stream, int *read_size,
                             void *destination, unsigned size)
{
    return func_0019d5e4(&stream->state, read_size, destination, size);
}
