#include "game/stream_manager.h"

static __inline struct StreamEvent *last_event(struct StreamEvent *event)
{
    struct StreamEvent *next;
    do {
        next = event->next;
        if (next)
            event = next;
    } while (next);
    return event;
}

static void consume_completion(struct StreamManager *manager, struct StreamEvent *event)
{
    /* ORIGINAL BUG: the serial is read before checking the dequeued pointer. */
    manager->completed_serial = event->serial;
    if (event) {
        manager->read_index = last_event(event)->start;
        manager->full = false;
        if (manager->write_index == manager->read_index) {
            manager->read_index = 0;
            manager->write_index = 0;
        }
    }
}

void func_00140904(struct StreamManager *manager)
{
    struct StreamEvent *event;
    while (func_001837c8(&manager->completions, &event))
        consume_completion(manager, event);
}
