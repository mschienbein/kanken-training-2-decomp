#include "game/functions.h"
#include "game/startup.h"
#include "game/resource_runtime.h"
#include "game/runtime.h"

struct EventResource;

extern void func_0019d3b4(struct ResourceEvent *event);

void func_001869f0(void)
{
    if (bss_0049db78.option_b && bss_0049db78.active && bss_0049db78.pending) {
        if (bss_0049db78.event) {
            if (bss_0049db78.event->active) {
                func_0019d3b4(bss_0049db78.event);
                --data_00242238.active_count;
            }
            bss_0049db78.event = 0;
        }
        if (bss_0049db78.allocation) {
            func_0019dc20(bss_0049db78.allocation);
            bss_0049db78.allocation = 0;
        }
        bss_0049db78.active = 0;
    }
}
