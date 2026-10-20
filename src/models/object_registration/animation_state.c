#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_animation_state_cleanup(struct AnimationState *object,
    struct AnimationState *(*destroy)(struct AnimationState *),
    void (*image_base)(void))
{
}
