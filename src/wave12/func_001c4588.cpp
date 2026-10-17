#include "game/resource_font.h"
/* func_001c4598 calls this same backend slot with a writable one-word result,
 * then passes that word to the backend's slot at 0x3c. */


extern "C" unsigned func_001c4588(StreamCapabilityForwarder *self, unsigned *value)
{
    return self->stream->query(value);
}
