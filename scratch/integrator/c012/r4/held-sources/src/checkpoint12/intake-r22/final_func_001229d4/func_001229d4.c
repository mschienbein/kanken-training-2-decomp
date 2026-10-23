#include "game/graphics_state.h"
#include "game/stream_manager.h"
struct CallbackReplacement { GraphicsCallback callback; unsigned index; };
/* [INFERENCE] Request/lock helper factoring does not recover original type or callee identity. */
static GraphicsCallback replace_callback(const struct CallbackReplacement *request, struct GraphicsContext *context)
{
    GraphicsCallback previous = 0;
    if (request->index >= 7) return previous;
    func_00192bd8(&context->mutex);
    previous = context->callbacks[request->index];
    context->callbacks[request->index] = request->callback;
    func_00195d74(&context->mutex);
    return previous;
}
GraphicsCallback func_001229d4(GraphicsCallback callback, unsigned index)
{
    struct CallbackReplacement request;
    request.callback = callback;
    request.index = index;
    return replace_callback(&request, data_002430f0.context);
}
