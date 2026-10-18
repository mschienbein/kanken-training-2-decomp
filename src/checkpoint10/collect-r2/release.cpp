#include "game/stream_manager.h"
#include "game/cleanup_registry.h"

namespace {
struct BufferReleaseEvent : StreamEvent {
    unsigned size;
};
static StreamEvent *reserve_event(StreamManager *manager, unsigned words)
{
    StreamEvent *event = static_cast<StreamEvent *>(func_0019147c(manager, words));
    event->start = manager->write_index;
    return event;
}
static void initialize_event(StreamEvent *event, unsigned char opcode, void *object)
{
    event->opcode = opcode;
    event->object = object;
}
}

/* INTEGRATOR: move to include/game/stream_commands.h. The complete signature
 * follows CleanupCallback in game/cleanup_registry.h and its allocator. */
extern "C" {
void func_001d9ca0(void *buffer, unsigned size, void *context);
}

void func_001d9ca0(void *buffer, unsigned size, void *context)
{
    StreamManager *manager = func_001916e8();
    BufferReleaseEvent *event = static_cast<BufferReleaseEvent *>(reserve_event(manager, 6));
    initialize_event(event, 0x31, buffer);
    event->size = size;
    func_00191458(manager, event);
}
