#include "game/resource_font.h"
/* [INFERENCE] Partial stream interface. The adjacent read implementations use
 * result codes and output parameters; the forwarding slot preserves both. */


extern "C" unsigned func_001c456c(StreamSizeForwarder *self,
                                  unsigned long long *size)
{
    return self->stream->get_size(size);
}
