#include "game/graphics_state.h"

/* Partial typed view of a context member beyond the currently shared layout. */

int func_001229bc(void)
{
    const struct GraphicsContext *context = data_002430f0.context;
    return context->status;
}
