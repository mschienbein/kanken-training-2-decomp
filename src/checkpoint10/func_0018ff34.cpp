/* [INFERENCE] The seek/write roles follow the caller's file offset and source
 * buffer. Uncalled virtual slots describe layout only; their signatures are
 * unrecovered and are not evidence for additional callable interfaces. */
#include "game/file_stream.h"

bool func_0018ff34(PositionedOutputStream *stream, const unsigned char *buffer,
                   int position, int buffer_offset, unsigned size)
{
    bool failed = false;
    int written;
    int result = stream->seek(position + buffer_offset);
    if ((unsigned)result & 0x80000000u) {
        failed = true;
    } else {
        result = stream->write(&written, buffer + buffer_offset, size, true);
        if ((unsigned)result & 0x80000000u)
            failed = true;
    }
    return failed;
}
