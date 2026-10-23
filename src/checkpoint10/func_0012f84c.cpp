#include "game/graphics_state.h"

extern "C" void func_0012f84c(GraphicsContext *context)
{
    {
        StreamLockGuard guard(&context->mutex);
        if (context->state == 2) {
            context->state = 0;
            return;
        }
        context->state = 1;
    }
    func_00195d94(&context->completion);
}
