#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_style_owner_cleanup(struct StyleOwner *object,
    struct StyleOwner *(*destroy)(struct StyleOwner *),
    void (*image_base)(void))
{
}
