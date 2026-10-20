#include "game/functions.h"
#include "game/resource_runtime.h"
#include "game/runtime.h"

struct EventResource;

extern void func_0019d3b4(struct ResourceEvent *event);

static __inline void release_event(struct ResourceEvent *event)
{
    if (event->active) {
        func_0019d3b4(event);
        --data_00242238.active_count;
    }
}

void func_001351bc(void)
{
    struct ResourceEvent *event = data_00242238.events;
    if (event) {
        int i = 0;
        if (data_00242238.count > 0) {
            do {
                release_event(event);
                ++event;
            } while (++i < data_00242238.count);
        }
        func_0019dc20(data_00242238.events);
        data_00242238.events = 0;
        data_00242238.count = 0;
    }
}
