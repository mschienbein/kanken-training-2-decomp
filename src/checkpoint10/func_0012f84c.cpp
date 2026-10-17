#include "game/synchronization.h"

/* INTEGRATOR: differs from include/game/graphics_state.h: GraphicsContext has
 * a mutex at 0, a completion event at 0x64, and a state byte at 0x76. */
struct GraphicsContext {
    StreamMutex mutex;
    unsigned char reserved0c[0x64 - 0x0c];
    SignalState completion;
    unsigned char reserved6c[0x0a];
    unsigned char state;
};
namespace {
class ScopedStreamLock {
    StreamMutex *mutex;
public:
    explicit ScopedStreamLock(StreamMutex *value) : mutex(value)
    {
        func_00192bd8(mutex);
    }
    ~ScopedStreamLock()
    {
        func_00195d74(mutex);
    }
};
}

extern "C" void func_0012f84c(GraphicsContext *context)
{
    {
        ScopedStreamLock guard(&context->mutex);
        if (context->state == 2) {
            context->state = 0;
            return;
        }
        context->state = 1;
    }
    func_00195d94(&context->completion);
}
