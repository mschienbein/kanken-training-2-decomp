#include "game/sound_track.h"
/* [INFERENCE] The pointer-table entry forwards a scalar query; the concrete
 * interface names and unsigned return interpretation are not retained. */

extern "C" unsigned int func_001d3f08(ForwardingStream *object)
{
    ForwardedStream *stream = object->stream;
    return stream->vtable->query(stream);
}
