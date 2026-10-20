#include "game/file_stream.h"

extern "C" CtrResult func_001c4540(struct FileStreamBridge *stream, unsigned int *written,
                       const void *buffer, unsigned int size, int flush)
{
    return func_001c5074(&stream->state, written, buffer, size, flush);
}
