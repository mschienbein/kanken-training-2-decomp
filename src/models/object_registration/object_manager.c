#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_object_manager_cleanup(struct ObjectManager *object,
    void (*destroy)(struct ObjectManager *), void (*image_base)(void))
{
}
