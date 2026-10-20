#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_stream_manager_cleanup(struct StreamManager *object,
    void (*destroy)(struct StreamManager *), void (*image_base)(void))
{
}
