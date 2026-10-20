#include "game/resource_runtime.h"
struct EventResource;

extern void func_0019d3b4(struct ResourceEvent *event);

void func_00183208(struct ResourceEvent *event)
{
    if (event->active) {
        func_0019d3b4(event);
        --data_00242238.active_count;
    }
}
