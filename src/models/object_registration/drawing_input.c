#include "game/functions.h"

/* [INFERENCE] Source-only typed no-op registration. The original call identity
 * was erased; no callback is stored or invoked and no helper earns credit. */
void register_drawing_input_cleanup(struct W10DrawingInput *object,
    struct W10DrawingInput *(*destroy)(struct W10DrawingInput *),
    void (*image_base)(void))
{
}
